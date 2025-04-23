#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

const float SCALE = 30.f;
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

sf::Vector2f toSFMLPos(const b2Vec2& pos) 
{
    return sf::Vector2f(pos.x * SCALE, WINDOW_HEIGHT - pos.y * SCALE);
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ejercicio 3");
    window.setFramerateLimit(60);

    b2World world(b2Vec2(0.f, -10.f));

    //Punto fijo

    b2BodyDef anchorDef;
    anchorDef.position.Set(13.f, 15.f);
    b2Body* anchor = world.CreateBody(&anchorDef);

    //Pelota

    b2BodyDef ballDef;
    ballDef.type = b2_dynamicBody;
    ballDef.position.Set(13.f, 10.f);
    b2Body* ball = world.CreateBody(&ballDef);

    b2CircleShape circle;
    circle.m_radius = 0.5f;

    b2FixtureDef fix;
    fix.shape = &circle;
    fix.density = 1.0f;
    fix.restitution = 0.5f;
    ball->CreateFixture(&fix);

    ball->SetLinearVelocity(b2Vec2(5.f, 0.f));      //toque para que no parezca una pelota estatica

    //Union pelota-punto fijo

    b2DistanceJointDef jointDef;
    jointDef.Initialize(anchor, ball, anchor->GetWorldCenter(), ball->GetWorldCenter());
    world.CreateJoint(&jointDef);

    //Visual

    sf::CircleShape ballShape(0.5f * SCALE);
    ballShape.setOrigin(0.5f * SCALE, 0.5f * SCALE);
    ballShape.setFillColor(sf::Color::Green);

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        world.Step(1.f / 60.f, 6, 2);

        window.clear();

        ballShape.setPosition(toSFMLPos(ball->GetPosition()));
        window.draw(ballShape);

        sf::Vertex line[] =
        {
            sf::Vertex(toSFMLPos(anchor->GetPosition()), sf::Color::White),
            sf::Vertex(toSFMLPos(ball->GetPosition()), sf::Color::White)
        };
        window.draw(line, 2, sf::Lines);

        window.display();
    }

    return 0;
}