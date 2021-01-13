#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModulePhysics3D.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"

#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

    BuildCircuit();

    App->camera->Position = { 0,150,200 };
    App->camera->LookAt({ 0,0,0 });

    timer = 0.0f;
    start = true;
    onceMusic = true;
    onceMusicIntro = true;
    onceFinished = true;

    lapCounter = -1;
    maxLaps = 3;

    lapFx = App->audio->LoadFx("Assets/Fx/lap.wav");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

    App->audio->UnloadFx(lapFx);

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
    // Render
    Plane p(0, 1, 0, 0);
    p.axis = true;
    p.Render();

    if (start)
    {
        if (onceMusicIntro)
        {
            App->audio->PlayMusic("Assets/Music/title.ogg",-1,0.0f);
            onceMusicIntro = false;
        }
    }

    if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
    {
        start = false;
    }

    for (uint n = 0; n < c_circuit.Count(); n++)
    {
        c_circuit[n]->Render();
    }

    if (start)
    {
        App->window->SetTitle("God Hand's RACECAR! Press Enter to play.");
        textManager.PrintTextOnMap(-50.0f, 100.0f, 0.0f, Red, Font::TIMES_ROMAN_24, "God Hand's RACECAR! Press Enter to play.");
    }
    else if (!App->player->victory && !App->player->lose)
    {
        if (onceMusic)
        {
            App->camera->cameraType = CameraType::NORMAL;
            App->player->ResetPosition();
            App->audio->PlayMusic("Assets/Music/game.ogg");
            onceMusic = false;
        }
        timer += dt;
        char title[80];
        sprintf_s(title, "Laps: %d / %d, Timer: %.1f", lapCounter, maxLaps, timer);
        App->window->SetTitle(title);

        textManager.PrintTextOnMap(App->player->GetX(), App->player->GetY() + 7.0f, App->player->GetZ(), White, Font::TIMES_ROMAN_24, title);
    }

    if (!start && (App->player->victory || App->player->lose))
    {
        if (App->audio->HasFinished())
        {
            App->audio->PlayMusic("Assets/Music/victoryLooped.ogg");
        }
        char title[80];
        if (App->player->victory)
        {
            if (onceFinished)
            {
                App->audio->PlayMusic("Assets/Music/victory.ogg", 0, 0.0f);
                onceFinished = false;
                finalCarPos.x = App->player->GetX();
                finalCarPos.y = App->player->GetY() + 7.0f;
                finalCarPos.z = App->player->GetZ();
            }
            sprintf_s(title, "You win! Your time has been %.1f. Press R to restart!", timer);
            textManager.PrintTextOnMap(finalCarPos.x, finalCarPos.y, finalCarPos.z, White, Font::TIMES_ROMAN_24, title);
        }
        else if (App->player->lose)
        {
            if (onceFinished)
            {
                App->audio->PlayMusic("Assets/Music/lose.ogg", 0, 0.0f);
                onceFinished = false;
                finalCarPos.x = App->player->GetX();
                finalCarPos.y = App->player->GetY() + 7.0f;
                finalCarPos.z = App->player->GetZ();
            }
            sprintf_s(title, "You lose... Your time has been %.1f. Press R to restart!", timer);
            textManager.PrintTextOnMap(finalCarPos.x, finalCarPos.y, finalCarPos.z, White, Font::TIMES_ROMAN_24, title);
        }
        App->window->SetTitle(title);
        if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
        {
            Reset();
        }
    }

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
    if (!start && body2 == App->player->vehicle)
    {
        if (body1 == finish_line)
        {
            if (lapCounter == maxLaps)
            {
                App->player->victory = true;
            }
            //else if (check_1 == true)
            else if (check_point1->isChecked == true)
            {
                CheckPoint();
                check_point1->isChecked = false;
                lapCounter++;
                finish_line->isChecked = true;
                App->audio->PlayFx(lapFx);
            }
        }
        else if (body1 == check_point1)
        {
            //if (check_line == true)
            if (finish_line->isChecked == true)
            {
                CheckPoint();
                finish_line->isChecked = false;
                check_point1->isChecked = true;
            }
        }
        else if (body1 == dead_zone || body1 == ob_dead_zone || body1 == ob_dead_zone_2 || body1 == ob_dead_zone_3)
        {
            //Go last checkpoint;
            GoLastCheckPoint();
        }
    }
}

void ModuleSceneIntro::CheckPoint()
{
    c_x = App->player->GetX();
    c_y = App->player->GetY();
    c_z = App->player->GetZ();
    App->player->vehicle->GetTransform(&c_matrix);
}

void ModuleSceneIntro::GoLastCheckPoint()
{
    App->player->vehicle->SetPos(c_x, c_y, c_z);
    if(finish_line->isChecked == true)
        c_matrix.rotate(360, vec3(0.0f, 1.0f, 0.0f));
    else if(check_point1->isChecked == true)
        c_matrix.rotate(270,vec3(0.0f, 1.0f, 0.0f));
    App->player->vehicle->SetTransform(&c_matrix);
    App->player->vehicle->vehicle->getRigidBody()->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
}

void ModuleSceneIntro::Reset()
{
    start = true;
    onceMusicIntro = true;
    timer = 0.0f;
    onceMusic = true;
    lapCounter = -1;
    maxLaps = 3;
    onceFinished = true;
    App->camera->cameraType = CameraType::EAGLE;
    App->player->victory = false;
    App->player->lose = false;
    App->player->ResetPosition();
    finish_line->isChecked = false;
    check_point1->isChecked = true;
}

void ModuleSceneIntro::BuildCircuit()
{
    const float size = 0.5f;
    const float height = 3.0f;
    const float pilonDistance = 0.3f;
    const float halfRoad = 10.0f;
    float previousRoadPos = 0.0f;
    p2Point<float> circleCenter;
    p2Point<float> roadPosition;

    roadPosition.x = 115.0f;
    roadPosition.y = 45.0f;
    previousRoadPos = CreateRoad(roadPosition, size, height, 15, pilonDistance, halfRoad, RoadType::VERTICAL_DOWN);

    circleCenter.x = 85.0f;
    circleCenter.y = previousRoadPos + roadPosition.y;
    CreateCurve(circleCenter, 30.0f, 90.0f, size, height, 35, pilonDistance, CurveType::BOTTOM_RIGHT);
    CreateCurve(circleCenter, 10.0f, 90.0f, size, height, 12, pilonDistance, CurveType::BOTTOM_RIGHT);

    roadPosition.x = circleCenter.x + 10.0f;
    roadPosition.y = previousRoadPos + 65.0f;
    previousRoadPos = CreateRoad(roadPosition, size, height, 50, pilonDistance, halfRoad, RoadType::HORIZONTAL_LEFT);

    circleCenter.x = 21.5f;
    circleCenter.y = -previousRoadPos - size;
    CreateCurve(circleCenter, 30.0f, 90.0f, size, height, 35, pilonDistance, CurveType::BOTTOM_LEFT);
    CreateCurve(circleCenter, 10.0f, 90.0f, size, height, 12, pilonDistance, CurveType::BOTTOM_LEFT);

    roadPosition.x = circleCenter.x - 10.0f;
    roadPosition.y = -previousRoadPos - size;
    previousRoadPos = CreateRoad(roadPosition, size, height * 3, 30, pilonDistance, halfRoad, RoadType::VERTICAL_UP);

    roadPosition.x = (halfRoad * 1.25f) - 1.0f;
    roadPosition.y = 27.0f;
    float positionY = -1.5f;
    CreateRamp(roadPosition, positionY, halfRoad * 1.85f, 20, RoadType::VERTICAL_UP);

    float xPos = 0.f;
    for (int n = 0; n < 14; n++)
    {
        Cylinder* c = new Cylinder(size, height);
        c->SetRotation(90.0f, { 0.0f,0.0f,1.0f });
        c_circuit.PushBack(c);
        c->SetPos(roadPosition.x - halfRoad + 1.5f + xPos, 0.f + height / 2, 2.0f);
        c->color = { 1,0.5,0,1 };
        App->physics->AddBody(*c, 0.0f);
        xPos += size + size + pilonDistance;
    }

    roadPosition.y = 2.0f;
    previousRoadPos = CreateRoad(roadPosition, size, height, 50, pilonDistance, halfRoad, RoadType::VERTICAL_UP);

    circleCenter.x = roadPosition.x - halfRoad * 3;
    circleCenter.y = previousRoadPos + size * 4;
    CreateCurve(circleCenter, 30.0f, 90.0f, size, height * 3, 35, pilonDistance, CurveType::TOP_RIGHT);
    CreateCurve(circleCenter, 10.0f, 90.0f, size, height, 12, pilonDistance, CurveType::TOP_RIGHT);

    roadPosition.x = circleCenter.x + 10.0f;
    roadPosition.y = previousRoadPos - halfRoad * 2 + size * 4;
    previousRoadPos = CreateRoad(roadPosition, size, height, 25, pilonDistance, halfRoad, RoadType::HORIZONTAL_LEFT);

    circleCenter.x = previousRoadPos - 17.25f;
    circleCenter.y = roadPosition.y - (halfRoad * 2);
    CreateCurve(circleCenter, 30.0f, 90.0f, size, height, 35, pilonDistance, CurveType::BOTTOM_LEFT);
    CreateCurve(circleCenter, 10.0f, 90.0f, size, height, 12, pilonDistance, CurveType::BOTTOM_LEFT);

    circleCenter.x += previousRoadPos - 7.5f;
    CreateCurve(circleCenter, 30.0f, 90.0f, size, height, 35, pilonDistance, CurveType::TOP_RIGHT);
    CreateCurve(circleCenter, 10.0f, 90.0f, size, height, 12, pilonDistance, CurveType::TOP_RIGHT);

    roadPosition.x = circleCenter.x + 10.0f;
    roadPosition.y = previousRoadPos - (halfRoad * 9) - size;
    previousRoadPos = CreateRoad(roadPosition, size, height, 25, pilonDistance, halfRoad, RoadType::HORIZONTAL_LEFT);

    circleCenter.x = previousRoadPos - (halfRoad * 9) + size * 3;
    circleCenter.y = roadPosition.y + (halfRoad * 2);
    CreateCurve(circleCenter, 30.0f, 90.0f, size, height, 35, pilonDistance, CurveType::TOP_LEFT);
    CreateCurve(circleCenter, 10.0f, 90.0f, size, height, 12, pilonDistance, CurveType::TOP_LEFT);
    
    roadPosition.x = previousRoadPos - (halfRoad * 10) + size * 3;
    roadPosition.y = circleCenter.y + size * 3;
    previousRoadPos = CreateRoad(roadPosition, size, height, 72, pilonDistance, halfRoad, RoadType::VERTICAL_DOWN);

    circleCenter.x = roadPosition.x + (halfRoad);
    circleCenter.y = -previousRoadPos + (halfRoad * 8.5) + (size);
    CreateCurve(circleCenter, 30.0f, 90.0f, size, height, 35, pilonDistance, CurveType::BOTTOM_LEFT);
    CreateCurve(circleCenter, 10.0f, 90.0f, size, height, 12, pilonDistance, CurveType::BOTTOM_LEFT);

    roadPosition.x = roadPosition.x + halfRoad * 2;
    roadPosition.y = -previousRoadPos + (halfRoad * 10.5) + size;
    previousRoadPos = CreateRoad(roadPosition, size, height, 40, pilonDistance, halfRoad, RoadType::HORIZONTAL_RIGHT);
    
    roadPosition.x = -previousRoadPos - halfRoad + (size * 4);
    previousRoadPos = CreateRoad(roadPosition, size, height * 3, 40, pilonDistance, halfRoad, RoadType::HORIZONTAL_RIGHT);

    roadPosition.x = -9.5f;
    roadPosition.y = (halfRoad * 1.3f) - 1.1f;
    CreateRamp(roadPosition, positionY, halfRoad * 1.85f, 20, RoadType::HORIZONTAL_RIGHT);

    float zPos = 0.f;
    for (int n = 0; n < 14; n++)
    {
        Cylinder* c = new Cylinder(size, height);
        c->SetRotation(90.0f, { 0.0f,0.0f,1.0f });
        c_circuit.PushBack(c);
        c->SetPos(30.0f, 0.f + height / 2, roadPosition.y - halfRoad + 1.5f + zPos);
        c->color = { 1,0.5,0,1 };
        App->physics->AddBody(*c, 0.0f);
        zPos += size + size + pilonDistance;
    }

    roadPosition.x = previousRoadPos - (halfRoad * 2.0f) - (size * 4);
    previousRoadPos = CreateRoad(roadPosition, size, height, 51, pilonDistance, halfRoad, RoadType::HORIZONTAL_RIGHT);

    circleCenter.x = previousRoadPos + (halfRoad * 2) - (size * 3);
    circleCenter.y = roadPosition.y + (halfRoad * 2);
    CreateCurve(circleCenter, 30.0f, 90.0f, size, height * 3, 35, pilonDistance, CurveType::TOP_RIGHT);
    CreateCurve(circleCenter, 10.0f, 90.0f, size, height, 12, pilonDistance, CurveType::TOP_RIGHT);

    roadPosition.x = 115.0f;
    roadPosition.y = 33.25f;
    previousRoadPos = CreateRoad(roadPosition, size, height, 9, pilonDistance, halfRoad, RoadType::VERTICAL_DOWN);

    //------------SENSORS----------------------------
    //----------Finish Line--------------------------
    c_finish.Size(30, 10, 1);
    finish_line = App->physics->AddBody(c_finish, 0);
    finish_line->SetPos(115, 0, 50);
    finish_line->GetTransform(&c_finish.transform);
    finish_line->SetAsSensor(true);
    finish_line->isChecked = false;
    finish_line->isDeath = false;
    finish_line->collision_listeners.add(this);

    //------Check point 1----------------------------
    c_ck1.Size(1, 10, 30);
    check_point1 = App->physics->AddBody(c_ck1, 0);
    check_point1->SetPos(-100, 0, -125);
    check_point1->GetTransform(&c_ck1.transform);
    check_point1->SetAsSensor(true);
    check_point1->isChecked = true;
    check_point1->isDeath = false;
    check_point1->collision_listeners.add(this);

    //------ Dead Zone ------------------------------
    c_dead.Size(40, 1, 25);
    dead_zone = App->physics->AddBody(c_dead, 0);
    dead_zone->SetPos(10, 0, 15);
    dead_zone->GetTransform(&c_dead.transform);
    dead_zone->SetAsSensor(true);
    dead_zone->isDeath = true;
    dead_zone->collision_listeners.add(this);

    //Out of bounds Dead Zone
    c_ob_dead.Size(150, 1, 100);
    ob_dead_zone = App->physics->AddBody(c_ob_dead, 0);
    ob_dead_zone->SetPos(98, 0, -50);
    ob_dead_zone->GetTransform(&c_ob_dead.transform);
    ob_dead_zone->SetAsSensor(true);
    ob_dead_zone->isDeath = true;
    ob_dead_zone->collision_listeners.add(this);

    c_ob_dead_2.Size(70, 1, 50);
    ob_dead_zone_2 = App->physics->AddBody(c_ob_dead_2, 0);
    ob_dead_zone_2->SetPos(63, 0, 48);
    ob_dead_zone_2->GetTransform(&c_ob_dead_2.transform);
    ob_dead_zone_2->SetAsSensor(true);
    ob_dead_zone_2->isDeath = true;
    ob_dead_zone_2->collision_listeners.add(this);

    c_ob_dead_3.Size(110, 1, 70);
    ob_dead_zone_3 = App->physics->AddBody(c_ob_dead_3, 0);
    ob_dead_zone_3->SetPos(-58, 0, -35);
    ob_dead_zone_3->GetTransform(&c_ob_dead_3.transform);
    ob_dead_zone_3->SetAsSensor(true);
    ob_dead_zone_3->isDeath = true;
    ob_dead_zone_3->collision_listeners.add(this);


    CreateBorders();
}

void ModuleSceneIntro::CreateBorders()
{
    Cube* border1 = new Cube(400.0f, 500.0f, 1.0f);
    c_circuit.PushBack(border1);
    border1->SetPos(0.0f, 250.0f, 200.0f);
    border1->color = { 0.67f, 0.84f, 0.9f, 1.0f };

    Cube* border2 = new Cube(400.0f, 500.0f, 1.0f);
    c_circuit.PushBack(border2);
    border2->SetPos(0.0f, 250.0f, -200.0f);
    border2->color = { 0.67f, 0.84f, 0.9f, 1.0f };

    Cube* border3 = new Cube(1.0f, 500.0f, 400.0f);
    c_circuit.PushBack(border3);
    border3->SetPos(200.0f, 250.0f, 0.0f);
    border3->color = { 0.67f, 0.84f, 0.9f, 1.0f };

    Cube* border4 = new Cube(1.0f, 500.0f, 400.0f);
    c_circuit.PushBack(border4);
    border4->SetPos(-200.0f, 250.0f, 0.0f);
    border4->color = { 0.67f, 0.84f, 0.9f, 1.0f };

    Cube* floor = new Cube(400.0f, 1.0f, 400.0f);
    c_circuit.PushBack(floor);
    floor->SetPos(0.0f, -1.0f, 0.0f);
    floor->color = { 0.56f, 0.93f, 0.56f, 1.0f };
}

float ModuleSceneIntro::CreateRoad(p2Point<float> init, float sizePilon, float heightPilon, int numPilons, float pilonDistance, float halfRoad, RoadType type)
{
    float zPos = 0.f;
    float xPos = 0.f;
    switch (type)
    {
    case RoadType::VERTICAL_DOWN:
        {
            for (int n = 0; n < numPilons; n++)
            {
                Cylinder* c = new Cylinder(sizePilon, heightPilon);
                c->SetRotation(90.0f, { 0.0f,0.0f,1.0f });
                c_circuit.PushBack(c);
                c->SetPos(init.x + halfRoad, 0.f + heightPilon / 2, init.y + zPos);
                c->color = { 1,0.5,0,1 };
                App->physics->AddBody(*c, 0.0f);
                zPos += sizePilon + sizePilon + pilonDistance;
            }

            zPos = 0.f;
            for (int n = 0; n < numPilons; n++)
            {
                Cylinder* c = new Cylinder(sizePilon, heightPilon);
                c->SetRotation(90.0f, { 0.0f,0.0f,1.0f });
                c_circuit.PushBack(c);
                c->SetPos(init.x - halfRoad, 0.f + heightPilon / 2, init.y + zPos);
                c->color = { 1,0.5,0,1 };
                App->physics->AddBody(*c, 0.0f);
                zPos += sizePilon + sizePilon + pilonDistance;
            }
            return zPos;
            break;
        }
    case RoadType::HORIZONTAL_RIGHT:
        {
            for (int n = 0; n < numPilons; n++)
            {
                Cylinder* c = new Cylinder(sizePilon, heightPilon);
                c->SetRotation(90.0f, { 0.0f,0.0f,1.0f });
                c_circuit.PushBack(c);
                c->SetPos(init.x + xPos, 0.f + heightPilon / 2, init.y + halfRoad);
                c->color = { 1,0.5,0,1 };
                App->physics->AddBody(*c, 0.0f);
                xPos += sizePilon + sizePilon + pilonDistance;
            }

            xPos = 0.f;
            for (int n = 0; n < numPilons; n++)
            {
                Cylinder* c = new Cylinder(sizePilon, heightPilon);
                c->SetRotation(90.0f, { 0.0f,0.0f,1.0f });
                c_circuit.PushBack(c);
                c->SetPos(init.x + xPos, 0.f + heightPilon / 2, init.y - halfRoad);
                c->color = { 1,0.5,0,1 };
                App->physics->AddBody(*c, 0.0f);
                xPos += sizePilon + sizePilon + pilonDistance;
            }
            return xPos;
            break;
        }
    case RoadType::VERTICAL_UP:
        {
            for (int n = 0; n < numPilons; n++)
            {
                Cylinder* c = new Cylinder(sizePilon, heightPilon);
                c->SetRotation(90.0f, { 0.0f,0.0f,1.0f });
                c_circuit.PushBack(c);
                c->SetPos(init.x + halfRoad, 0.f + heightPilon / 2, init.y + zPos);
                c->color = { 1,0.5,0,1 };
                App->physics->AddBody(*c, 0.0f);
                zPos -= sizePilon + sizePilon + pilonDistance;
            }

            zPos = 0.f;
            for (int n = 0; n < numPilons; n++)
            {
                Cylinder* c = new Cylinder(sizePilon, heightPilon);
                c->SetRotation(90.0f, { 0.0f,0.0f,1.0f });
                c_circuit.PushBack(c);
                c->SetPos(init.x - halfRoad, 0.f + heightPilon / 2, init.y + zPos);
                c->color = { 1,0.5,0,1 };
                App->physics->AddBody(*c, 0.0f);
                zPos -= sizePilon + sizePilon + pilonDistance;
            }
            return zPos;
            break;
        }
    case RoadType::HORIZONTAL_LEFT:
        {
            for (int n = 0; n < numPilons; n++)
            {
                Cylinder* c = new Cylinder(sizePilon, heightPilon);
                c->SetRotation(90.0f, { 0.0f,0.0f,1.0f });
                c_circuit.PushBack(c);
                c->SetPos(init.x + xPos, 0.f + heightPilon / 2, init.y + halfRoad);
                c->color = { 1,0.5,0,1 };
                App->physics->AddBody(*c, 0.0f);
                xPos -= sizePilon + sizePilon + pilonDistance;
            }

            xPos = 0.f;
            for (int n = 0; n < numPilons; n++)
            {
                Cylinder* c = new Cylinder(sizePilon, heightPilon);
                c->SetRotation(90.0f, { 0.0f,0.0f,1.0f });
                c_circuit.PushBack(c);
                c->SetPos(init.x + xPos, 0.f + heightPilon / 2, init.y - halfRoad);
                c->color = { 1,0.5,0,1 };
                App->physics->AddBody(*c, 0.0f);
                xPos -= sizePilon + sizePilon + pilonDistance;
            }
            return xPos;
            break;
        }
    }

}

void ModuleSceneIntro::CreateCurve(p2Point<float> circleCenter, float radious, float referenceAngle, float sizePilon, float heightPilon, int numPilons, float pilonDistance, CurveType type)
{
    float ang = 0.0f;
    switch (type)
    {
    case CurveType::BOTTOM_LEFT:
        {
            for (int n = 0; n < numPilons; n++)
            {
                Cylinder* c = new Cylinder(sizePilon, heightPilon);
                c->SetRotation(90.0f, { 0.0f,0.0f,1.0f });
                c_circuit.PushBack(c);
                c->SetPos(circleCenter.x + 10.0f + (radious * -cos(DEGTORAD * ang)), 0.f + heightPilon / 2, circleCenter.y + (radious * sin(DEGTORAD * ang)));
                c->color = { 1,0.5,0,1 };
                App->physics->AddBody(*c, 0.0f);
                ang += referenceAngle / numPilons;
            }
            break;
        }
    case CurveType::BOTTOM_RIGHT:
        {
            for (int n = 0; n < numPilons; n++)
            {
                Cylinder* c = new Cylinder(sizePilon, heightPilon);
                c->SetRotation(90.0f, { 0.0f,0.0f,1.0f });
                c_circuit.PushBack(c);
                c->SetPos(circleCenter.x + 10.0f + (radious * cos(DEGTORAD * ang)), 0.f + heightPilon / 2, circleCenter.y + (radious * sin(DEGTORAD * ang)));
                c->color = { 1,0.5,0,1 };
                App->physics->AddBody(*c, 0.0f);
                ang += referenceAngle / numPilons;
            }
            break;
        }
    case CurveType::TOP_LEFT:
        {
            for (int n = 0; n < numPilons; n++)
            {
                Cylinder* c = new Cylinder(sizePilon, heightPilon);
                c->SetRotation(90.0f, { 0.0f,0.0f,1.0f });
                c_circuit.PushBack(c);
                c->SetPos(circleCenter.x + 10.0f + (radious * -cos(DEGTORAD * ang)), 0.f + heightPilon / 2, circleCenter.y + (radious * -sin(DEGTORAD * ang)));
                c->color = { 1,0.5,0,1 };
                App->physics->AddBody(*c, 0.0f);
                ang += referenceAngle / numPilons;
            }
            break;
        }
    case CurveType::TOP_RIGHT:
        {
            for (int n = 0; n < numPilons; n++)
            {
                Cylinder* c = new Cylinder(sizePilon, heightPilon);
                c->SetRotation(90.0f, { 0.0f,0.0f,1.0f });
                c_circuit.PushBack(c);
                c->SetPos(circleCenter.x + 10.0f + (radious * cos(DEGTORAD * ang)), 0.f + heightPilon / 2, circleCenter.y + (radious * -sin(DEGTORAD * ang)));
                c->color = { 1,0.5,0,1 };
                App->physics->AddBody(*c, 0.0f);
                ang += referenceAngle / numPilons;
            }
            break;
        }
    }
}

void ModuleSceneIntro::CreateRamp(p2Point<float> position, float height, float length, float angle, RoadType type)
{
    switch (type)
    {
    case RoadType::VERTICAL_DOWN:
        {
            Cube* r = new Cube(length, 5.0f, 5.0f);
            r->SetPos(position.x, height, position.y);
            r->SetRotation(-angle, { 1,0,0 });
            c_circuit.PushBack(r);
            r->color = { 0,0,1,1 };
            App->physics->AddBody(*r, 0.0f);
            break;
        }
    case RoadType::HORIZONTAL_RIGHT:
        {
            Cube* r = new Cube(5.0f, 5.0f, length);
            r->SetPos(position.x, height, position.y);
            r->SetRotation(angle, { 0,0,1 });
            c_circuit.PushBack(r);
            r->color = { 0,0,1,1 };
            App->physics->AddBody(*r, 0.0f);
            break;
        }
    case RoadType::VERTICAL_UP:
        {
            Cube* r = new Cube(length, 5.0f, 5.0f);
            r->SetPos(position.x, height, position.y);
            r->SetRotation(angle, { 1,0,0 });
            c_circuit.PushBack(r);
            r->color = { 0,0,1,1 };
            App->physics->AddBody(*r, 0.0f);
            break;
        }
    case RoadType::HORIZONTAL_LEFT:
        {
            Cube* r = new Cube(5.0f, 5.0f, length);
            r->SetPos(position.x, height, position.y);
            r->SetRotation(-angle, { 0,0,1 });
            c_circuit.PushBack(r);
            r->color = { 0,0,1,1 };
            App->physics->AddBody(*r, 0.0f);
            break;
        }
    }
}