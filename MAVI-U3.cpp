#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <vector>

const float SCALE = 30.f;
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

sf::Vector2f toSFMLPos(const b2Vec2& pos) 
{
    return sf::Vector2f(pos.x * SCALE, WINDOW_HEIGHT - pos.y * SCALE);
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ejercicio 5");
    window.setFramerateLimit(60);

    b2World world(b2Vec2(0.f, -10.f));
    bool activo = false;

    //Cajas

    auto createBox = [&](float x, float y, float width, float height) -> b2Body*
    {
        b2BodyDef bodyDef;
        bodyDef.position.Set(x, y);
        bodyDef.type = b2_staticBody;
        b2Body* body = world.CreateBody(&bodyDef);

        b2PolygonShape shape;
        shape.SetAsBox(width / 2, height / 2);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        body->CreateFixture(&fixtureDef);

        return body;
    };

    //Suelo

    b2BodyDef groundDef;
    groundDef.position.Set(13.f, 1.f);
    b2Body* ground = world.CreateBody(&groundDef);
    b2PolygonShape groundShape;
    groundShape.SetAsBox(13.f, 1.f);
    ground->CreateFixture(&groundShape, 0.f);

    //Partes/miembros del ragdoll

    b2Body* torso = createBox(13.f, 5.f, 1.f, 2.f);
    b2Body* head = createBox(13.f, 7.5f, 1.f, 1.f);
    b2Body* leftArm = createBox(12.f, 5.f, 0.5f, 2.f);
    b2Body* rightArm = createBox(14.f, 5.f, 0.5f, 2.f);
    b2Body* leftLeg = createBox(12.5f, 3.f, 0.5f, 2.f);
    b2Body* rightLeg = createBox(13.5f, 3.f, 0.5f, 2.f);

    std::vector<b2Body*> cuerpos = { torso, head, leftArm, rightArm, leftLeg, rightLeg };
    std::vector<b2Joint*> joints;

    //Uniones
    
    struct Union 
    {
        b2Body* a;
        b2Vec2 AnchorA;
        b2Body* b;
        b2Vec2 AnchorB;
    };

    std::vector<Union> uniones = 
    {
        {torso,     b2Vec2(0.f, 1.f),    head,     b2Vec2(0.f, -0.5f)},
        {torso,     b2Vec2(-0.5f, 0.9f), leftArm,  b2Vec2(0.f, 1.f)},
        {torso,     b2Vec2(0.5f, 0.9f),  rightArm, b2Vec2(0.f, 1.f)},
        {torso,     b2Vec2(-0.4f, -1.f), leftLeg,  b2Vec2(0.f, 1.f)},
        {torso,     b2Vec2(0.4f, -1.f),  rightLeg, b2Vec2(0.f, 1.f)}
    };

    for (const auto& u : uniones)
    {
        b2Vec2 worldAnchorA = u.a->GetWorldPoint(u.AnchorA);
        b2Vec2 worldAnchorB = u.b->GetWorldPoint(u.AnchorB);
        b2Vec2 jointAnchor = 0.5f * (worldAnchorA + worldAnchorB);

        b2RevoluteJointDef jointDef;
        jointDef.bodyA = u.a;
        jointDef.bodyB = u.b;
        jointDef.localAnchorA = u.a->GetLocalPoint(jointAnchor);
        jointDef.localAnchorB = u.b->GetLocalPoint(jointAnchor);
        jointDef.collideConnected = false;

        joints.push_back(world.CreateJoint(&jointDef));
    }

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && !activo)
            {
                for (auto body : cuerpos)
                {
                    body->SetType(b2_dynamicBody);
                }

                //Fuerza lateral al activar

                b2Vec2 impulso(5.f, 0.f);
                torso->ApplyLinearImpulseToCenter(impulso, true);

                activo = true;
            }
        }

        world.Step(1.f / 60.f, 8, 3);
        window.clear();

        //Dibujar cuerpo

        for (auto body : cuerpos) 
        {
            for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) 
            {
                if (f->GetType() == b2Shape::e_polygon) 
                {
                    sf::ConvexShape shape;
                    b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
                    shape.setPointCount(poly->m_count);
                    for (int i = 0; i < poly->m_count; ++i) 
                    {
                        b2Vec2 vertex = body->GetWorldPoint(poly->m_vertices[i]);
                        shape.setPoint(i, toSFMLPos(vertex));
                    }
                    shape.setFillColor(sf::Color::Green);
                    window.draw(shape);
                }
            }
        }

        //Dibujar uniones
        
            for (auto joint : joints) 
            {
                b2Body* bodyA = joint->GetBodyA();
                b2Body* bodyB = joint->GetBodyB();
                b2Vec2 anchor = joint->GetAnchorA();

                sf::Vertex lineA[] =
                {
                    sf::Vertex(toSFMLPos(bodyA->GetPosition()), sf::Color::White),
                    sf::Vertex(toSFMLPos(anchor), sf::Color::White)
                };
                window.draw(lineA, 2, sf::Lines);

                sf::Vertex lineB[] =
                {
                    sf::Vertex(toSFMLPos(bodyB->GetPosition()), sf::Color::White),
                    sf::Vertex(toSFMLPos(anchor), sf::Color::White)
                };
                window.draw(lineB, 2, sf::Lines);
            }
        

        window.display();
    }

    return 0;
}