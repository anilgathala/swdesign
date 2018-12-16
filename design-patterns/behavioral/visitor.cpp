// visitor design pattern
// we have a visitor class which executes "visit()" code for each element in a hierarchy
// example: file system hierarchy, print every file recursively

// there is a hirarchy of objects upon which a variety of operations can be performed
// capture these variety of operations in a visitor interface
// now define the different visitors you'd like to implement
// once you have them, call root-of-hierarchy.accept(visitor_of_a_particular_type);
// inside accept() method:
// -- visitor->appropriate_method(); // method

#include <iostream>
#include <string>
#include <vector>

const int dir_overhead_bytes = 20;

class File {
private:
    std::string name_;
    int size_;
public:
    File(std::string name, int size): name_(name), size_(size) {}
    std::string name() {
        return name_;
    }
    int size() {
        return size_;
    }
};

class IFileNode;
class FileNode;
class DirectoryNode;
class IVisitor;

class IFileNode {
public:
    virtual std::string name() = 0;
    virtual int size() = 0;
    virtual void accept(IVisitor* visitor) = 0;
    virtual bool is_directory() = 0;
};

class IVisitor {
public:
   virtual void visit_file_node(FileNode* file) = 0; 
   virtual void visit_dir_node(DirectoryNode* dir) = 0;
};


class FileNode: public IFileNode {
private:
    File *file_;
public:
    FileNode(File* file): file_(file) {}

    std::string name() {
        return file_->name();
    }

    int size() {
        return file_->size();
    }

    void accept(IVisitor* visitor) {
        visitor->visit_file_node(this);
    }

    bool is_directory() {
        return false;
    }
};

class DirectoryNode: public IFileNode {
private:
    std::string name_;
    int overhead_bytes_;
    std::vector<IFileNode*> file_list_;
public:
    DirectoryNode(std::string name, int overhead_bytes): name_(name), overhead_bytes_(overhead_bytes) {}

    void add_file(std::string name, int size) {
        file_list_.push_back(new FileNode(new File(name, size)));
    }

    void add_dir(std::string name) {
        file_list_.push_back(new DirectoryNode(name, dir_overhead_bytes));
    }

    std::string name() {
        return name_;
    }

    int size() {
        // int sz = overhead_bytes_;
        // for (auto f: file_list_) {
        //     sz += f->size();
        // }
        // return sz;
        return overhead_bytes_;
    }

    void accept(IVisitor* visitor) {
        visitor->visit_dir_node(this);
    }

    std::vector<IFileNode*> children() {
        return file_list_;
    }

    bool is_directory() {
        return false;
    }

    int overhead_bytes()
    {
        return overhead_bytes_;
    }
};


//  Visitor #1: traverse hierarchy and print every file/dir name along with size
class PrintVisitor: public IVisitor {
private:
    int level;
public:
    PrintVisitor(): level(0) {}
    void visit_file_node(FileNode* file) {
        std::cout << file->name() << " " << file->size() << " Kb" << std::endl;
    }

    void visit_dir_node(DirectoryNode* dir) {
        for (int i = 0; i < level; i++) {
            std::cout << " ";
        }
        auto children = dir->children();
        for (auto c: children) {
           if (c->is_directory()) {
               visit_dir_node((DirectoryNode*) c);
           } else {
               visit_file_node((FileNode*) c);
           }
        }
        std::cout << std::endl;
    }
};


//  Visitor #2: traverse hierarchy and get the total size
class DiskUsage: public IVisitor {
private:
    int size_;
public:
    DiskUsage() : size_(0) {}
    void visit_file_node(FileNode* file) {
        size_ += file->size();
    }

    void visit_dir_node(DirectoryNode* dir) {
        auto children = dir->children();
        for (auto c: children) {
           if (c->is_directory()) {
               size_ += dir->overhead_bytes();
               visit_dir_node((DirectoryNode*) c);
           } else {
               visit_file_node((FileNode*) c);
           }
        }
    }

    int size() {
        return size_;
    }
};

int main()
{
    DirectoryNode home("/home", dir_overhead_bytes);
    home.add_dir("workspace");
    home.add_file("log.txt", 2048);
    home.add_file("notes.txt", 1024);

    home.accept(new PrintVisitor());
    auto du = new DiskUsage();
    home.accept(du);
    std::cout << "total disk usage: " << /* total size */ du->size() << " Kb" << std::endl;
    return 0;
}