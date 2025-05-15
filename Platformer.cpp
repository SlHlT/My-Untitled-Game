#include <iostream>
#include <algorithm>
#include <functional>
#include <limits>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

const int LEVEL_WIDTH = 25;
const int LEVEL_HEIGHT = 19;
const int TILE_SIZE = 32;
const int level1Contents[LEVEL_HEIGHT][LEVEL_WIDTH] = {
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 1, 1, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};



class Player{
    public:
        sf::Vector2f vel = sf::Vector2f(0, 0);
        sf::Vector2f acc = sf::Vector2f(0.75, 1);

        sf::Vector2f position = sf::Vector2f(340, 340);
        sf::Vector2f size = sf::Vector2f(32, 32);

        sf::RectangleShape collideBox;

    void changePosition(float fElaspedTime){
        position += vel;
    }
    
    void gravity(float fElaspedTime){
        vel.y += 2.2 * fElaspedTime;
    }

    void jump(float fElaspedTime){
        vel.y = 0;
        vel.y -= 350 * fElaspedTime;
    }

    void holdingA(float fElaspedTime){
        vel.x -= 64 * fElaspedTime;
        if(vel.x < -256 * fElaspedTime){
            vel.x = -256 * fElaspedTime;
        }
    }

    void holdingD(float fElaspedTime){
        vel.x += 64 * fElaspedTime;
        if(vel.x > 256 * fElaspedTime){
            vel.x = 256 * fElaspedTime;
        }
    }
};

void TileRenderer(sf::RenderWindow &window, sf::Sprite &sprite, int x, int y);
void levelDrawer(sf::RenderWindow &window);
bool RayVsRect(const sf::Vector2f& ray_Origin, const sf::Vector2f& ray_Angle, const sf::RectangleShape& target,
sf::Vector2f& contact_point, sf::Vector2f& contact_normal, float& t_hit_near);
bool PlayerVsRect(const Player& in, const sf::RectangleShape& in_rect, const sf::RectangleShape& target, sf::Vector2f& contact_point, sf::Vector2f& contact_normal, float& contact_time, float fElaspedTime);
bool solidTile(int x, int y);

bool TileCollider(sf::RectangleShape& plrRect, Player& player, float& fElaspedTime, sf::RenderWindow &window);

int main(){
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");
    //window.setFramerateLimit(4);
    window.setKeyRepeatEnabled(false);

    sf::View view(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(800, 600)));
    bool holdingA = false;
    bool holdingD = false;

    Player player;

    while(window.isOpen()){  
        window.clear(sf::Color(25, 226, 237));   
        sf::Clock clock;
        //clock.reset();
        float fElaspedTime;

        view.setCenter(sf::Vector2f(player.position.x, 268));
        window.setView(view);

        while(const std::optional event = window.pollEvent()){

            if(const auto* resized = event->getIf<sf::Event::Resized>()){
                view.setSize(sf::Vector2f(resized->size));
            }

            if(event->is<sf::Event::Closed>()){
                window.close();
            }

            if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if(keyPressed->scancode == sf::Keyboard::Scan::W || keyPressed->scancode == sf::Keyboard::Scan::Space || keyPressed->scancode == sf::Keyboard::Scan::Up){
                    player.jump(fElaspedTime);
                }

                if(keyPressed->scancode == sf::Keyboard::Scan::A || keyPressed->scancode == sf::Keyboard::Scan::Left){
                    holdingA = true;
                }

                if(keyPressed->scancode == sf::Keyboard::Scan::D || keyPressed->scancode == sf::Keyboard::Scan::Right){
                    holdingD = true;
                }
            }

            if(const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()){
                if(keyReleased->scancode == sf::Keyboard::Scan::A || keyReleased->scancode == sf::Keyboard::Scan::Left){
                    holdingA = false;
                }

                if(keyReleased->scancode == sf::Keyboard::Scan::D || keyReleased->scancode == sf::Keyboard::Scan::Right){
                    holdingD = false;
                }
            }
        }
        player.collideBox.setSize(player.size);
        player.collideBox.setPosition(player.position);

        //player.gravity();
        if(holdingA == true){
            player.holdingA(fElaspedTime);
        }
        if(holdingD == true){
            player.holdingD(fElaspedTime);
        }
        player.changePosition(fElaspedTime);
        
        player.gravity(fElaspedTime);
        if(!TileCollider(player.collideBox, player, fElaspedTime, window)){
            
        }

        levelDrawer(window);
        
        window.draw(player.collideBox);
        window.display();
        fElaspedTime = clock.getElapsedTime().asSeconds();
    }
    return 0;
}

void levelDrawer(sf::RenderWindow &window){
    sf::Texture grassText("Sprites/Tiles/Grass.png");
    sf::Sprite grassSprite(grassText);

    sf::Texture leftGrassText("Sprites/Tiles/LeftGrass.png");
    sf::Sprite leftGrassSprite(leftGrassText);

    sf::Texture topLeftCornerGrassText("Sprites/Tiles/TopLeftCorrnerGrass.png");
    sf::Sprite topLeftCornerGrassSprite(topLeftCornerGrassText);
    
    sf::Texture BottomLeftCornerGrassText("Sprites/Tiles/BottomLeftCorrnerGrass.png");
    sf::Sprite BottomLeftCornerGrassSprite(BottomLeftCornerGrassText);

    sf::Texture rightGrassText("Sprites/Tiles/RightGrass.png");
    sf::Sprite rightGrassSprite(rightGrassText);

    sf::Texture topRightCornerGrassText("Sprites/Tiles/TopRightCorrnerGrass.png");
    sf::Sprite topRightCornerGrassSprite(topRightCornerGrassText);
    
    sf::Texture bottomRightCornerGrassText("Sprites/Tiles/BottomRightCornerGrass.png");
    sf::Sprite bottomRightCornerGrassSprite(bottomRightCornerGrassText);

    sf::Texture bottomGrassText("Sprites/Tiles/BottomGrass.png");
    sf::Sprite bottomGrassSprite(bottomGrassText);

    sf::Texture dirtText("Sprites/Tiles/Dirt.png");
    sf::Sprite dirtSprite(dirtText);

    sf::Texture placeHolderText("Sprites/Tiles/Placeholder.png");
    sf::Sprite placeHolderSprite(placeHolderText);



    for(int x = 0; x < LEVEL_WIDTH; x++){

        for(int y = 0; y < LEVEL_HEIGHT; y++){

            switch(level1Contents[y][x]){
                case 0:
                    continue;
                    break;
                case 1:
                    TileRenderer(window, grassSprite, x, y);
                    break;
                case 2:
                    TileRenderer(window, leftGrassSprite, x, y);
                    break;
                case 3:
                    TileRenderer(window, topLeftCornerGrassSprite, x, y);
                    break;
                case 4:
                    TileRenderer(window, BottomLeftCornerGrassSprite, x, y);
                    break;
                case 5:
                    TileRenderer(window, rightGrassSprite, x, y);
                    break;
                case 6:
                    TileRenderer(window, topRightCornerGrassSprite, x, y);
                    break;
                case 7:
                    TileRenderer(window, bottomRightCornerGrassSprite, x, y);
                    break;
                case 8:
                    TileRenderer(window, bottomGrassSprite, x, y);
                    break;
                case 9:
                    TileRenderer(window, dirtSprite, x, y);
                    break;
                default:
                    TileRenderer(window, placeHolderSprite, x, y);
            }
        }
    }
    return;
};

void TileRenderer(sf::RenderWindow &window, sf::Sprite &sprite, int x, int y){
    sprite.setPosition(sf::Vector2f((x * TILE_SIZE), (y * TILE_SIZE)));
    window.draw(sprite);
    return;
}

bool solidTile(int x, int y){
    if(x < 0 || y < 0 || x >= LEVEL_WIDTH || y >= LEVEL_HEIGHT){
        return true;
    }
    return level1Contents[y][x] == 1 || level1Contents[y][x] == 2 || level1Contents[y][x] == 3 || level1Contents[y][x] == 4 || level1Contents[y][x] == 5 || level1Contents[y][x] == 6 || level1Contents[y][x] == 7 || level1Contents[y][x] == 8 || level1Contents[y][x] == 9;
}

bool TileCollider(sf::RectangleShape& plrRect, Player& player, float& fElaspedTime, sf::RenderWindow &window){
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(32, 32));
    bool collided;
    std::string sideCollided;
    sf::Vector2f cp, cn;
    float ct;
    for(int y = 0; y < LEVEL_WIDTH; y++){
        for(int x = 0; x < LEVEL_HEIGHT; x++){
            if(solidTile(x, y)){
                cp = sf::Vector2f(0, 0);
                cn = sf::Vector2f(0, 0);
                ct = 0;
                rect.setPosition(sf::Vector2f((x * TILE_SIZE), (y * TILE_SIZE)));
            
                collided = PlayerVsRect(player, plrRect, rect, cp, cn, ct, fElaspedTime);


                if(collided == true){

                    
                    if(cn.x == 1){
                        sideCollided = "Left";
                        player.position = sf::Vector2f((x * TILE_SIZE - 10), player.position.y);
                    }else if(cn.x == -1){
                        sideCollided = "Right";
                        player.position = sf::Vector2f((x * TILE_SIZE + 10) - TILE_SIZE, player.position.y);
                    }else if(cn.y == 1){
                        sideCollided = "Top";
                        player.position = sf::Vector2f(player.position.x, (y * TILE_SIZE));
                    }else if(cn.y == -1){
                        sideCollided = "Bottom";
                        sf::Vector2f(player.position.x, (y * TILE_SIZE) - TILE_SIZE);
                    }

                    // 

                    // player.vel = sf::Vector2f(player.vel.x, 0);

                    player.vel += sf::Vector2f(
                        cn.x * std::abs(player.vel.x),
                        cn.y * std::abs(player.vel.y)
                    ) * (1.0f - ct);
                    player.vel += cn * fElaspedTime;
                }
                
            }   
        }
    }
    return collided;
}

bool RayVsRect(const sf::Vector2f& ray_Origin, const sf::Vector2f& ray_Angle, const sf::RectangleShape& target,
    sf::Vector2f& contact_point, sf::Vector2f& contact_normal, float& t_hit_near){

    contact_point = {0, 0};
    contact_normal = {0, 0};

    sf::Vector2f inv_dir;
    inv_dir.x = (ray_Angle.x != 0) ? 1.0f / ray_Angle.x : std::numeric_limits<float>::infinity();
    inv_dir.y = (ray_Angle.y != 0) ? 1.0f / ray_Angle.y : std::numeric_limits<float>::infinity();

    sf::Vector2f target_pos = target.getPosition();
    sf::Vector2f target_size = target.getSize();
    sf::Vector2f t_near = (target_pos - ray_Origin);
    sf::Vector2f t_far = (target_pos + target_size - ray_Origin);

    t_near = t_near.componentWiseDiv(inv_dir);
    t_far = t_far.componentWiseDiv(inv_dir);  

    if (t_near.x > t_far.x){
        std::swap(t_near.x, t_far.x);
    }
    if (t_near.y > t_far.y){
        std::swap(t_near.y, t_far.y);
    }

    if(t_near.x > t_far.y || t_near.y > t_far.x){
        return false;
    }

    t_hit_near = std::max(t_near.x, t_near.y);
    float t_hit_far = std::min(t_far.x, t_far.y);

    if (t_hit_far < 0){
        return false;
    }

    contact_point = ray_Origin + t_hit_near * ray_Angle;

    if(t_near.x > t_near.y){
        if (ray_Angle.x < 0){
            contact_normal = {1, 0};
        }else if(ray_Angle.x > 0){
            contact_normal = {-1, 0};
        }
    }else if(t_near.x < t_near.y){
        if (ray_Angle.y < 0){
            contact_normal = {0, 1};
        }else if(ray_Angle.y > 0){
            contact_normal = {0, -1};
        }
    }
    return true;
}

bool PlayerVsRect(const Player& in, const sf::RectangleShape& in_rect, const sf::RectangleShape& target, sf::Vector2f& contact_point, sf::Vector2f& contact_normal, float& contact_time, float fElaspedTime){
    if(in.vel.x == 0 && in.vel.y == 0){
        return false;
    }

    sf::RectangleShape expanded_target;
    expanded_target.setPosition(target.getPosition() - sf::Vector2f(in.size.x / 2, in.size.y / 2));
    expanded_target.setSize(target.getSize() + in.size);

    if(RayVsRect(in.position + (sf::Vector2f(in.size.x / 2, in.size.y / 2)), in.vel * fElaspedTime, expanded_target, contact_point, contact_normal, contact_time)){

        if(contact_time <= 1.0f && contact_time >= 0.0f){
            return true;
        }
    }
    return false;
}