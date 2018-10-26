#include <iostream>
#include <chrono>
#include <thread>

//
// Open Closed Principle
//
// There is some sound advice here on when not to use this principle:
// http://joelabrahamsson.com/a-simple-example-of-the-openclosed-principle/
//

class IMovieSceneSet {
public:
    // arrange for all the props
    virtual void arrange_properties() = 0;
    // set the scene lighting
    virtual void set_lighting() = 0;
    // is this an outdoor scene
    virtual bool outdoor() = 0;
    // is the set ready to shoot ?
    virtual bool ready() = 0;
};

class TrainSet: public IMovieSceneSet {
public:
    // arrange for all the props
    void arrange_properties() override {
        std::cout << " Train Scene, props: load numbered seats, overhead luggage shelves " << std::endl;
        props_ready = true;
    }
    // set the scene lighting
    void set_lighting() override {
        std::cout << " Train Scene, light: dim lights" << std::endl;
        lights_ready = true;
    }
    // is this an outdoor scene
    bool outdoor() override {
        return false;
    }
    // is the set ready to shoot ?
    bool ready() override {
        return (props_ready && lights_ready);
    }
private:
    bool props_ready;
    bool lights_ready;
};

class SkiingSet: public IMovieSceneSet {
public:
    // arrange for all the props
    void arrange_properties() override {
        std::cout << " Skiing Scene, props: load sledges, skii equipment, camera trolley " << std::endl;
        props_ready = true;
    }
    // set the scene lighting
    void set_lighting() override {
        std::cout << " Skiing Scene, light: sun umberallas" << std::endl;
        lights_ready = true;
    }
    // is this an outdoor scene
    bool outdoor() override {
        return true;
    }
    // is the set ready to shoot ?
    bool ready() override {
        return (props_ready && lights_ready);
    }
private:
    bool props_ready;
    bool lights_ready;
};


class MovieSceneSetComposer {
public:
    // OCP: Rather than coding this method for a specific movie-scene-set
    //      we're coding for all kinds of scenes.
    //      Now this class is open for extension but closed for modification
    void compose_scene_set(IMovieSceneSet& set) {

        if (set.outdoor()) {
            std::cout << " put note to actors about voice modulation!" << std::endl;
        }
        set.arrange_properties();
        set.set_lighting();
        std::cout << " adjust camera!" << std::endl;
        while(!set.ready()) {
            std::cout << " indefinte wait for the set to be ready" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::cout << " Action!" << std::endl; 
    }
};

int main()
{
    std::cout << "Action Movie!" << std::endl;

    TrainSet train_set;
    SkiingSet ski_set;

    MovieSceneSetComposer composer;

    std::cout << "Movie shoot begin: " << std::endl;
    std::cout << "Ski Scene: " << std::endl;
    composer.compose_scene_set(ski_set);
    std::cout << "Train Scene: " << std::endl;
    composer.compose_scene_set(train_set);
    std::cout << "Movie shoot end! " << std::endl;
    
    return 0;
}