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
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ejercicio 4");
    window.setFramerateLimit(60);

    b2World world(b2Vec2(0.f, 0.f));

    //--------------------------------
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

    //-----------------------------------------

    //Cuadrado

    b2BodyDef boxDef;
    boxDef.type = b2_dynamicBody;
    boxDef.position.Set(13.f, 10.f);
    b2Body* box = world.CreateBody(&boxDef);
    box->SetFixedRotation(true);
    box->SetLinearDamping(5.0f);

    b2PolygonShape square;
    square.SetAsBox(0.5f, 0.5f);

    b2FixtureDef fix;
    fix.shape = &square;
    fix.density = 1.0f;
    fix.friction = 0.3f;
    box->CreateFixture(&fix);

    //Visual

    sf::RectangleShape squareShape(sf::Vector2f(1.f * SCALE, 1.f * SCALE));
    squareShape.setOrigin(0.5f * SCALE, 0.5f * SCALE);
    squareShape.setFillColor(sf::Color::Cyan);

    //Fuerza y direccion

    const float FORCE = 15.0f;

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 
        {
            box->ApplyForceToCenter(b2Vec2(0.f, FORCE), true);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) 
        {
            box->ApplyForceToCenter(b2Vec2(0.f, -FORCE), true);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
        {
            box->ApplyForceToCenter(b2Vec2(-FORCE, 0.f), true);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
        {
            box->ApplyForceToCenter(b2Vec2(FORCE, 0.f), true);
        }

        world.Step(1.f / 60.f, 6, 2);

        window.clear();
        squareShape.setPosition(toSFMLPos(box->GetPosition()));
        window.draw(squareShape);
        window.display();
    }

    return 0;
}