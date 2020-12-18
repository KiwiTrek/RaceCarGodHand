#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

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

    const float size = 0.5f;
    const float height = 1.0f;
    const float pilonDistance = 0.3f;

    float xPos = 0.f;
    for (int n = 0; n < 20; n++)
    {
        Cylinder* c = new Cylinder(size,height);
        c->SetRotation(90.0f, { 0.0f,0.0f,1.0f });
        c_circuit.PushBack(c);
        c->SetPos(xPos, 0.f + height/2, 2.5f);

        //TODO 2: Link all the spheres with your P2P constraints

        xPos += size + size + pilonDistance;
    }


	App->camera->Move(vec3(10.0f, 10.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
    //for (uint n = 0; n < c_circuit.Count(); n++)
    //    c_circuit[n].

    // Render
    Plane p(0, 1, 0, 0);
    p.axis = true;
    p.Render();

    for (uint n = 0; n < c_circuit.Count(); n++)
    {
        c_circuit[n]->Render();
    }

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

