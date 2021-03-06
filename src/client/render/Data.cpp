/**********************************************************
Data.cpp
Displays game state data and buttons to interact with them.
***********************************************************/
#include "Data.h"
#include "Render.h"
#include <json.hpp>
#include <iostream>

using json = nlohmann::json;

namespace render {
    Data::Data () {}
    Data::Data (Render * mainRender, ToState * state, ToEngine * engine) {
        this->mainRender=mainRender;
        this->state = state;
        this->engine = engine;
        // text
        font.loadFromFile("./res/DejaVuSerifCondensed.ttf");
        text.setFont(font);
        text.setString(data);
        text.setCharacterSize(20);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        // button
        // view
        float width = 0.3;
        float height = 0.8;
        float widthText = width*(float)mainRender->getWindow()->getSize().x;
        float heightText = height*(float)mainRender->getWindow()->getSize().y;
        view.setViewport(sf::FloatRect(0, 0.1, width, height));
         // view size
        view.setSize(widthText,heightText-0.1);
        view.setCenter(widthText/2, (heightText-0.1)/2);
        // frame
        frame.setFillColor(sf::Color(128,128,128,255));
        frame.setSize(view.getSize());
        sf::Color color(160,160,160,255);
        buttons.push_back(new Button(mainRender, 0, heightText+0.1, "", color, sf::Vector2i(widthText, mainRender->getWindow()->getSize().y-heightText)));
    }
    Data::~Data () {
        for (Button *button : buttons) {
            delete button;
        }
    }
    void Data::select(Types type,std::string id) {
        // reset the view if the user selected a different data to show
        if (this->type!=type || this->id!=id) {
            view.setCenter(view.getSize().x/2, view.getSize().y/2);
            text.setPosition(0,0);
        }
        //set global variables
        this->type = type;
        this->id = id;
        // select data type
        float width = view.getSize().x;
        float top = view.getViewport().top;
        float height = view.getViewport().height;
        float spaceV = (1-top-height)*mainRender->getWindow()->getSize().y;
        float y1 = mainRender->getWindow()->getSize().y*(height+top);
        float left = view.getViewport().left;
        float x1 = left*mainRender->getWindow()->getSize().x;
        sf::Color color(160,160,160,255);
        // create buttons
        if (type==Types::Character) {
            for (Button *button : buttons) {
                delete button;
            }
            buttons.clear();
            buttons.push_back(new Button(mainRender, x1, y1, "Relations", color, sf::Vector2i(width, spaceV/3)));
            buttons.push_back(new Button(mainRender, x1, y1+spaceV/3, "Alliance", color, sf::Vector2i(width/2, spaceV/3)));
            buttons.push_back(new Button(mainRender, x1+width/2, y1+spaceV/3, "Surrender", color, sf::Vector2i(width/2, spaceV/3)));
            buttons.push_back(new Button(mainRender, x1, y1+2*spaceV/3, "War", color, sf::Vector2i(width/2, spaceV/3)));
            buttons.push_back(new Button(mainRender, x1+width/2, y1+2*spaceV/3, "Peace", color, sf::Vector2i(width/2, spaceV/3)));
            try {
                if(this->id == "255")
                    this->data = "No one owns the Black Sea\n";
                else
                {
                    json j = state->fetchCharacterDataFromColor(this->id);
                    this->data = "Name: "+j["name"].get<std::string>()+"\n"+"Dynasty name: "+j["dynastyName"].get<std::string>()+"\n"
                    +"Age: "+j["age"].dump()+"\n"+"Traits: ";
                    for (auto& trait : j["traits"]) {
                        this->data+=trait.get<std::string>()+" ";
                    }
                    this->data=this->data+"\n"+"Statistics: "+"\n"
                    +"Diplomacy: "+j["diplomacy"].dump()+"\n"+"Stewardship: "+j["stewardship"].dump()+"\n"
                    +"Martial: "+j["martial"].dump()+"\n"+"Intrigue: "+j["intrigue"].dump()+"\n"+"\n"+"Alive: "
                    +j["alive"].dump()+"\n"+"Prestige: "+j["prestige"].dump()+"\n"+"Gold: "+j["gold"].dump()+"\n"
                    +"Plot active: "+j["hasPlot"].dump()+"\nClaims: "+j["claims"].dump(2);
                }
            }
            catch(const std::exception& e) {
                std::cerr << e.what() <<std::endl;
                throw std::runtime_error("Incorrect data for character "+id);
            }
                
        }
        if (type==Types::Province) {
            for (Button *button : buttons) {
                delete button;
            }
            buttons.clear();
            buttons.push_back(new Button(mainRender, x1, y1, "Levy", color, sf::Vector2i(width/2, spaceV/2)));
            buttons.push_back(new Button(mainRender, x1+width/2, y1, "Claim", color, sf::Vector2i(width/2, spaceV/2)));
            buttons.push_back(new Button(mainRender, x1, y1+spaceV/2, "", color, sf::Vector2i(width, spaceV/2)));
            try {
                json j = state->fetchProvinceDataFromColor(this->id);
                if(j["id"] == "sea")
                    this->data = j["name"];
                else
                {
                    this->data = "Name: "+j["name"].get<std::string>()+"\n"+"Development: "+j["development"].dump()+"\n"
                    +"Prosperity: "+j["prosperity"].dump()+"\n"+"Levy: ";
                    for (json::iterator info=j["levy"].begin();info!=j["levy"].end();info++){
                        this->data = this->data+info.key()+": "+info.value().dump()+" ";
                    }
                    this->data = this->data+"\n"+"Tax income: "+j["taxIncome"].dump()+"\n"+"Siege status: "+j["siegeStatus"].dump()+"\n"
                    +"Sieging army: "+j["siegingArmy"].get<std::string>()+"\n"+"Controlled by: "+j["controlledBy"].get<std::string>();
                }
            }
            catch(const std::exception& e) {
                std::cerr << e.what() <<std::endl;
                throw std::runtime_error("Incorrect data for province "+id);
            }
        }
        
        if (type==Types::Army) {
            for (Button *button : buttons) {
                delete button;
            }
            buttons.clear();
            buttons.push_back(new Button(mainRender, x1, y1, "", color, sf::Vector2i(width, spaceV)));
            try {
                json j = state->fetchArmyData(this->id);
                this->data = "Owner: "+j["ownerCharacter"].get<std::string>()+"\n"+"Levies: ";
                for(auto& levy : j["levies"]){
                    this->data+=levy;
                }
                this->data = this->data+"\n"+"Current province: "+j["currentProvince"].get<std::string>()+"\n"+"Current battle: "
                +j["currentBattle"].get<std::string>()+"\n"+"Orders: "+j["orders"].dump(2)+"\n";
            }
            catch(const std::exception& e) {
                std::cerr << e.what() <<std::endl;
                throw std::runtime_error("Incorrect data for army "+id);
            }
        }

        if (type==Types::Relations) {
            for (Button *button : buttons) {
                delete button;
            } 
            buttons.clear();
            buttons.push_back(new Button(mainRender, x1, y1, "Character", color, sf::Vector2i(width, spaceV/3)));
            buttons.push_back(new Button(mainRender, x1, y1+spaceV/3, "", color, sf::Vector2i(width, 2*spaceV/3)));
            buttons.push_back(new Button(mainRender, x1, y1+spaceV/3, "", color, sf::Vector2i(width, 2*spaceV/3)));
            buttons.push_back(new Button(mainRender, x1, y1+spaceV/3, "", color, sf::Vector2i(width, 2*spaceV/3)));
            buttons.push_back(new Button(mainRender, x1, y1+spaceV/3, "", color, sf::Vector2i(width, 2*spaceV/3)));
            try {
                json j = state->fetchAllRelationsData();
                json character = state->fetchCharacterDataFromColor(this->id);
                this->data = "Character: "+character["name"].get<std::string>()+"\n"+"Relations: "+"\n";
                for(json::iterator it = j.begin(); it!= j.end(); ++it)
                {
                    std::string characterA = it.value()["characterA"].get<std::string>();
                    std::string characterB = it.value()["characterB"].get<std::string>();
                    if(characterA==character["id"] || characterB==character["id"]) {
                        if(characterA==character["id"]) {
                            json characterBData = state->fetchCharacterData(characterB);
                            this->data +=characterBData["name"].get<std::string>()+" "+characterBData["dynastyName"].get<std::string>()+": ";
                        }
                        else if(characterB==character["id"]){
                            json characterAData = state->fetchCharacterData(characterA);
                            this->data +=characterAData["name"].get<std::string>()+" "+characterAData["dynastyName"].get<std::string>()+": ";
                        }
                        switch(it.value()["type"].get<int>()){
                            case 0:
                                //type = non_aggression;
                                this->data +="no aggression\n";
                                break;
                            case 1:
                                //type = alliance;
                                this->data +="alliance\n";
                                break;
                            case 2:
                                //type = friendship;
                                this->data +="friendship\n";
                                break;
                            case 3:
                                //type = rivalry;
                                this->data +="rivalry\n";
                                break;
                            case 4:
                                //type = war;
                                this->data +="war\n";
                                break;
                            default:
                                throw std::string("Error: invalid relation type.");
                        }
                    }
                }
            }
            catch(const std::exception& e) {
                std::cerr << e.what() <<std::endl;
                throw std::runtime_error("Incorrect data for relations");
            }
        }
        // insert endline characters into the text so that if fits the window
        float charSize = text.getCharacterSize()*1.9*view.getViewport().width;
        float lineMaxSize = width/charSize;
        size_t length = this->data.length();
        float lineSize = 0;
        size_t lastSpace = 0; //position of last space in text
        size_t lastSpaceInLine = 0; //position of last space in line
        // modify the text while iterating over it, not very clean but it works
        for (size_t i=0;i<length;i++) {
            lineSize+=1;
            if (this->data[i] == ' ') {
                    lastSpace=i; 
                    lastSpaceInLine = lineSize;
            }
            if (this->data[i] == '\n') {
                    lineSize=0;
                    lastSpace=0; 
            }
            else if (lineSize>lineMaxSize) {
                if (lastSpace !=0) {
                    this->data.replace(lastSpace,1,1,'\n');
                    lastSpace=0;
                    lineSize-=lastSpaceInLine; 
                }
                else if (data[i+1]==' ') {
                    this->data.erase(i+1,1);
                }
                else {
                    this->data.insert(i,1,'\n');
                    lineSize=0;
                    length++;
                }
            }
        }
        text.setString(this->data);
    }
    void  Data::draw() {
        for (Button *button : buttons) {
            button->draw();
        }
            // change view
            mainRender->getWindow()->setView(this->view);
            // draw panel
            mainRender->Render::getWindow()->draw(frame);
            mainRender->Render::getWindow()->draw(text);
            // restore view
            mainRender->Render::getWindow()->setView(mainRender->Render::getWindow()->getDefaultView());
    }

    bool Data::contains(int x, int y) {
        for (Button *button : buttons) {
            if (button->contains(x,y)) {
                return true;
            }
        }
        sf::FloatRect hitbox = view.getViewport();
        sf::Vector2u size = mainRender->getWindow()->getSize();
        int left = size.x*hitbox.left;
        int right = left+size.x*hitbox.width;
        int top = size.y*hitbox.top;
        int bottom = top+size.y*hitbox.height;
        if ((x>left) & (x<right)) {
            if ((y>top) & (y<bottom)) {
                return true;
            }
        }
        return false;
    }
    void Data::click(int x,int y) {
        for (Button *button : buttons) {
            if (button->contains(x,y)) {
                handleButtons(button->getDescription());
            }
        }
    } 
    void Data::handleButtons(std::string button) {
        json j;
        if (button =="") {}
        else if (type==Types::Relations) {
            if (button=="Character") {this->select(Types::Character, id);}
        }
        else if (type==Types::Character) {
            if (button=="Relations") {this->select(Data::Relations, id);}
            else if (button =="Alliance") {}
            else if (button=="Surrender") {}
            else if (button=="War") {
            }
            else if (button=="Peace") {
            }
            j["command"] = button;
            j["colorCode"] = std::stoul(id);
            engine->addCommand(button, j.dump());
        }
        else if (type==Types::Province) {
            if (button=="Levy") {}
            else if (button =="Claim") {}
            j["command"] = button;
            j["colorCode"] = std::stoul(id);
            engine->addCommand(button, j.dump());
        }
        
        else if (type==Types::Army) {
            j["command"] = button;
            j["colorCode"] = id;
            engine->addCommand(button, j.dump());
        }
    }

    void Data::scroll(int deltaY) {
        float frameH = frame.getSize().y;
        float newTextPos = text.getPosition().y-deltaY;
        float newTextBottom = text.getPosition().y+text.getLocalBounds().height-deltaY;
        if ((newTextPos<=0) & (newTextBottom>=frameH)){
            text.setPosition(0, newTextPos);
        }
        else if ((text.getPosition().y<=0) & (newTextPos>0)) {
            text.setPosition(0, 0);
        }
        else if (((text.getPosition().y+text.getLocalBounds().height)>=frameH) & (newTextBottom<frameH)) {
            text.setPosition(0, frameH-text.getLocalBounds().height);
        }
    }

    void Data::update() {
        if (this->id !="") {
            select(this->type, this->id);
        }
    }
    
}