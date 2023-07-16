#include <SFML/Graphics.hpp>
#include <sstream>


class game{
private:
    // window
    sf::Vector2f resolution {1280,720};
    sf::RenderWindow window;
    sf::Font font;
    sf::Text pause_text;
    sf::Text score_text;


    //game setting
    sf::Vector2f player_size {150,20};
    sf::Vector2f ball_size {20,20};
    sf::Vector2f ball_reset_position {resolution.x/2 - ball_size.x/2 ,resolution.y/2};
    sf::Vector2f player_reset_position {resolution.x/2 - player_size.x/2 , resolution.y - 60};
    sf::Vector2f player_pos_now;
    sf::Vector2f ball_pos_now;


    sf::RectangleShape player;
    sf::RectangleShape ball;

    float ball_velocity_x {4};
    float ball_velocity_y {4};
    short pause_flag {1}; //for pause the game
    const unsigned int FPS {60};
    static const sf::Time TimePerFrame;
    static long score; 


public:
    void draw();
    void close_event();
    void pause_sign();

    // draw game    
    void background();
    void ball_move();
    void player_move();

    // the main loop would be in the run function
    void run();


};

long game::score = 0;

void game::player_move(){
    player_pos_now = (player.getPosition());

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
       if(player_pos_now.x >= 10)
           player.move(-10.f , 0);  

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
       if(player_pos_now.x <= resolution.x - 10 - player_size.x) 
           player.move(10.f , 0);
}



void game::ball_move(){
    ball_pos_now = (ball.getPosition());

    if(ball_pos_now.y <=0) 
        ball_velocity_y *= -1;
    if(ball_pos_now.x <=0 || ball_pos_now.x > resolution.x - ball_size.x)
        ball_velocity_x *= -1;

    if(ball_pos_now.x >= player_pos_now.x && ball_pos_now.x <= player_pos_now.x + player_size.x &&
       (ball_pos_now.y >= player_pos_now.y - ball_size.y && ball_pos_now.y < player_pos_now.y + player_size.y )){
        ball_velocity_y *= -1;
        ball_pos_now.y = player_pos_now.y - ball_size.y;
        score += 1;

        if(ball_pos_now.x < player_pos_now.x + player_size.x * 0.6
        && ball_pos_now.x > player_pos_now.x + player_size.x * 0.4)
            ball_velocity_x *= 0.2;
        else
            ball_velocity_x +=1;
    }

    ball_pos_now.x += ball_velocity_x;  
    ball_pos_now.y += ball_velocity_y;  

    if(ball_pos_now.y > resolution.y){
        ball_pos_now = ball_reset_position;
        score = 0;
    }

    ball.setPosition(ball_pos_now);
}



void game::draw(){ 
    std::stringstream ss;
    ss << "SCORE: " << score;
    score_text.setString(ss.str());

    window.clear();

    window.draw(score_text);
    window.draw(player);
    window.draw(ball);

    window.display();

} 

void game::pause_sign(){
    pause_text.setFillColor(sf::Color::White);
    pause_text.setFont(font);
    pause_text.setPosition(resolution.x / 2 - 55, resolution.y / 2);
    pause_text.setString("PAUSE");

    window.draw(pause_text);
    window.display();
}

void game::close_event(){
    sf::Event event;
    while (window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            window.close();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();
    }

}        

void game::run(){
    font.loadFromFile("/Users/TSHung/Downloads/ProjectC/font/Arial.ttf");
    window.create(sf::VideoMode(resolution.x,resolution.y), "Program");
    window.setFramerateLimit(FPS);

    score_text.setFillColor(sf::Color::White);
    score_text.setFont(font);
    score_text.setPosition(10,680);

    player.setPosition(player_reset_position);
    player.setSize(player_size);
    ball.setPosition(ball_reset_position);
    ball.setSize(ball_size);

     while(window.isOpen())
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        { 
            pause_sign();
            pause_flag *= -1;
            sf::sleep(sf::seconds(0.2));
        }

        close_event();
        if(pause_flag > 0)
        {  
            player_move();
            ball_move();
            draw();
        }    
    }
}



int main(void){

    game gg;
    gg.run();

    return 0;
} 




