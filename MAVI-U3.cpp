#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>
#include <cmath>

const float SCALE = 30.f;
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

sf::Vector2f toSFMLPos(const b2Vec2& pos) 
{
    return sf::Vector2f(pos.x * SCALE, WINDOW_HEIGHT - pos.y * SCALE);
}

b2Vec2 toBox2DPos(const sf::Vector2f& pos) 
{
    return b2Vec2(pos.x / SCALE, (WINDOW_HEIGHT - pos.y) / SCALE);
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ejercicio 2");
    window.setFramerateLimit(60);

    b2World world(b2Vec2(0.f, -10.f));


    //---------------------------------
    //Bordes

    b2BodyDef wallDef;
    wallDef.position.Set(0, 0);
    b2Body* wall = world.CreateBody(&wallDef);

    b2EdgeShape edge;
    b2FixtureDef edgeFixture;
    edgeFixture.shape = &edge;

    //Piso

    edge.SetTwoSided(b2Vec2(0, 0), b2Vec2(WINDOW_WIDTH / SCALE, 0));
    wall->CreateFixture(&edgeFixture);

    //Techo

    edge.SetTwoSided(b2Vec2(0, WINDOW_HEIGHT / SCALE), b2Vec2(WINDOW_WIDTH / SCALE, WINDOW_HEIGHT / SCALE));
    wall->CreateFixture(&edgeFixture);

    //Izquierdo

    edge.SetTwoSided(b2Vec2(0, 0), b2Vec2(0, WINDOW_HEIGHT / SCALE));
    wall->CreateFixture(&edgeFixture);

    //Derecho

    edge.SetTwoSided(b2Vec2(WINDOW_WIDTH / SCALE, 0), b2Vec2(WINDOW_WIDTH / SCALE, WINDOW_HEIGHT / SCALE));
    wall->CreateFixture(&edgeFixture);

    //------------------------------------------

    //Pelotas

    b2BodyDef def;
    def.type = b2_dynamicBody;

    def.position.Set(10.f, 10.f);
    b2Body* ball1 = world.CreateBody(&def);

    def.position.Set(13.f, 10.f);
    b2Body* ball2 = world.CreateBody(&def);

    b2CircleShape circle;
    circle.m_radius = 0.5f;

    b2FixtureDef fix;
    fix.shape = &circle;
    fix.density = 1.0f;
    fix.restitution = 0.5f;

    ball1->CreateFixture(&fix);
    ball2->CreateFixture(&fix);

    //"Resorte"

    b2DistanceJointDef spring;
    spring.Initialize(ball1, ball2, ball1->GetWorldCenter(), ball2->GetWorldCenter());

    world.CreateJoint(&spring);

    //Visuales

    sf::CircleShape ballShape(0.5f * SCALE);
    ballShape.setOrigin(0.5f * SCALE, 0.5f * SCALE);
    ballShape.setFillColor(sf::Color::Yellow);

    b2Body* selectedBall = nullptr;
    bool dragging = false;

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) 
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                b2Vec2 box2DPos = toBox2DPos(mousePos);

                if ((ball1->GetPosition() - box2DPos).Length() < circle.m_radius)
                    selectedBall = ball1;
                else if ((ball2->GetPosition() - box2DPos).Length() < circle.m_radius)
                    selectedBall = ball2;

                dragging = selectedBall != nullptr;
            }

            if (event.type == sf::Event::MouseButtonReleased) 
            {
                selectedBall = nullptr;
                dragging = false;
            }
        }

        if (dragging && selectedBall) 
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            selectedBall->SetTransform(toBox2DPos(mousePos), selectedBall->GetAngle());
            selectedBall->SetLinearVelocity(b2Vec2(0, 0));
        }

        world.Step(1.f / 60.f, 6, 2);

        window.clear();

        ballShape.setPosition(toSFMLPos(ball1->GetPosition()));
        window.draw(ballShape);
        ballShape.setPosition(toSFMLPos(ball2->GetPosition()));
        window.draw(ballShape);

        window.display();
    }

    return 0;
}