// Nistor Vlad Alexandru
// Assignment 3.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <sstream>
using namespace tle;

// Model number constants
const int checkpointNumber = 5;
const int isleNumber = 9;
const int wallNumber = 6;
const int waterTank1Number = 4;

//Game speeds
const float kGameSpeed = 1;
float frameTime;
const float cameraRotationSpeed = 800.0f;
const float cameraSpeed = 30.0f;
const float hovercarSpeed = 0.001f;
const float hovercarTurningSpeed = 70.0f;

// Game scale ( from units to metres )
const float gameScale = 1.2f;

// Enums

enum gameStates {game_start, screen_prompt, stage1, stage2, stage3, stage4, stage5, race_finished, game_over};
enum timers { timer_1, timer_2, timer_3, timer_go,timer_boost_duration,timer_boost_cooldown, boost_unavailable,
			  timer_stage1_complete, timer_stage2_complete,timer_stage3_complete, timer_stage4_complete, timer_stage5_complete, timer_game_end };

enum cameraStates {manual, automatic};

enum lastKeyHit{W, S};

// Function declarations
void ResetCameraPosition(I3DEngine* &myEngine, ICamera* &myCamera);

bool sphere2sphere(float sphere1xPosition, float sphere1zPosition, float sphere1Radius,
				   float sphere2xPosition, float sphere2zPosition, float sphere2Radius);

bool sphere2box(float spherexPosition, float spherezPosition, float sphereRadius,
				float boxxPosition,    float boxzPosition,    float boxWidth,	float boxDepth);

bool point2box(float pointXPosition, float pointZPosition, float boxXPosition, float boxZPosition, float boxWidth, float boxDepth);

// Structs
struct vector2D
{
	float x;
	float z;
};

vector2D scalar(float scalar, vector2D vector)
{
	return {scalar * vector.x, scalar * vector.z};
}

vector2D sum3(vector2D vector1, vector2D vector2, vector2D vector3)
{
	return { vector1.x + vector2.x + vector3.x, vector1.z + vector2.z + vector3.z };
}

// Model sizes ( collisions )
float hovercarRadius = 2.8f;
float checkpointWallRadius = 1.1f;
float checkpointClearWidth = 15.6f;
int wallWidth = 5;
int wallDepth = 23;
int waterTankRadius = 4;


void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media

	myEngine->AddMediaFolder("F:\\My files\\University files\\CO1301 - Games Concepts\\Assignment 3\\Models"); // memory stick folder
	myEngine->AddMediaFolder( "C:\\Users\\VANistor\\Desktop\\University files\\CO1301 - Games Concepts\\Assignment 3\\Models" ); // home folder

	// Camera creation
	ICamera* myCamera = myEngine->CreateCamera(kManual);
	ResetCameraPosition(myEngine, myCamera);

	// Mesh Loading
	IMesh* checkpointMesh = myEngine->LoadMesh("Checkpoint.x");
	IMesh* isleMesh = myEngine->LoadMesh("IsleStraight.x");
	IMesh* wallMesh = myEngine->LoadMesh("Wall.x");
	IMesh* skyboxMesh = myEngine->LoadMesh("Skybox 07.x");
	IMesh* groundMesh = myEngine->LoadMesh("ground.x");
	IMesh* hovercarMesh = myEngine->LoadMesh("race2.x");
	IMesh* waterTank1Mesh = myEngine->LoadMesh("TankSmall1.x");
	IMesh* waterTank2Mesh = myEngine->LoadMesh("TankSmall2.x");
	IMesh* trapMesh = myEngine->LoadMesh("Tribune1.x");
	IMesh* bombMesh = myEngine->LoadMesh("Flare.x");
	
	// Checkpoint positions
	int checkpointXPositions[checkpointNumber] = { 0, 10,  25, 25, 25 };
	int checkpointZPositions[checkpointNumber] = { 0, 120, 56, -50, -90 };

	// Isle positions
	int isleXPositions[isleNumber] = { -10, 10, -10, 10, -10, 10, 10, 10, 10 };
	int isleZPositions[isleNumber] = { 40, 40, 56, 56, 72, 72, -10, -26, -42 };

	// Wall positions
	int wallXPositions[wallNumber] = { -10, 10, -10, 10, 10, 10 };
	int wallZPositions[wallNumber] = { 48, 48, 64 ,64, -18, -34 };

	// Water Tank_1 Positions
	int waterTank1XPositions[waterTank1Number] = { -15, -5, 30, 40};
	int waterTank1ZPositions[waterTank1Number] = { 100, 135, 135, 100};

	// Checkpoint wall positions
	float checkpointWallXPositions[checkpointNumber * 2] = {-8.7, 8.7, 10,    10,     16.3, 33.7, 16.3, 33.3};
	float checkpointWallZPositions[checkpointNumber * 2] = {0,    0,   111.3, 128.7,  56,   56,   -50,  -50};

	// Hover car starting positions
	float hovercarStartingXPos = 0;
	float hovercarStartingZPos = -50;

	// Water tank obstacle positions
	int waterTankObstacleXPosition = 30;
	int waterTankObstacleYPosition = -7;
	int waterTankObstacleZPosition = 20;

	// Model Creation and Initialization
	IModel* checkpoint[checkpointNumber];
	for (int i = 0; i < checkpointNumber; i++)
	{
		checkpoint[i] = checkpointMesh->CreateModel(checkpointXPositions[i], 0, checkpointZPositions[i]);
		if (i == 1)
		{
			checkpoint[i]->RotateY(90);
		}
	}
	IModel* isle[isleNumber];
	for (int i = 0; i < isleNumber; i++)
	{
		isle[i] = isleMesh->CreateModel(isleXPositions[i], 0, isleZPositions[i]);
	}
	IModel* wall[wallNumber];
	for (int i = 0; i < wallNumber; i++)
	{
		wall[i] = wallMesh->CreateModel(wallXPositions[i], 0, wallZPositions[i]);
	}
	IModel* ground = groundMesh->CreateModel();
	IModel* skybox = skyboxMesh->CreateModel(0,-960,0);
	IModel* hovercar = hovercarMesh->CreateModel(hovercarStartingXPos, 0, hovercarStartingZPos);

	IModel* waterTank1[waterTank1Number];
	for (int i = 0; i < waterTank1Number; i++)
	{
		waterTank1[i] = waterTank1Mesh->CreateModel(waterTank1XPositions[i], 0, waterTank1ZPositions[i]);
	}
	
	IModel* waterTankObstacle = waterTank2Mesh->CreateModel(waterTankObstacleXPosition, waterTankObstacleYPosition, waterTankObstacleZPosition);
	waterTankObstacle->RotateZ(-10);

	// Keeping track of the camera rotation
	float cameraRotationUpLimit = -10.0f;
	float cameraRotationDownLimit = 5.0f;
	float currentCameraRotation = 0.0f;

	// Hovercar movement vectors
	vector2D momentum{ 0.0f,0.0f };
	vector2D thrust{ 0.0f,0.0f };
	vector2D drag{ 0.0f,0.0f };

	float matrix[4][4];

	float dragCoefficient = -0.005f;

	//const string skinName = "redskin.jpg";

	// Declaring the font type used in the UI
	IFont* myFont = myEngine->LoadFont("Times New Roman", 28);
	ISprite* backdrop = myEngine->CreateSprite("ui_modified.jpg", 150, 640);

	// Initializing the game state
	gameStates currentGameState = game_start;

	// Game state display positions
	float gameStateXPosition = 240;
	float gameStateZPosition = 675;

	// Middle Text display positions
	//float midTextXPosition = 550;
	//float midTextZPosition = 280;

	// Initializing timers for screen prompts
	float timer = 0;
	timers currentTimer = timer_3;

	// Saving the hovercar positions
	float hovercarXPosition;
	float hovercarZPosition;

	// Hovercar health
	int hovercarStartingHealth = 100;
	int hovercarDamageHit = 1;
	int hovercarCurrentHealth = 100;

	// Hovercar speeds
	int hovercar_metres_seconds;
	int hovercar_kilometres_hour;

	//Initializing camera state
	cameraStates currentCameraState = manual;
	bool thirdPerson = true;
	
	//Hovercar rotation
	float currentHovercarRotation = 0.0f;

	//Frame timer
	int frameTimer = 0;
	float oldHovercarX = hovercar->GetX();
	float oldHovercarZ = hovercar->GetZ();

	// Last key hit
	lastKeyHit lastKeyHit = W;

	// Boost timer
	timers boostTimer = timer_boost_duration;
	int boostDuration = 3;
	int boostCooldown = 5;
	float currentBoostTimer = 0.0f;

	// World edges
	int worldEdge = 140;





	frameTime = myEngine->Timer();

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		//Calculating the time between this and previous frame
		frameTime = myEngine->Timer();

		// Saving old hovercar position
		if (frameTimer < 20)
		{
			oldHovercarX = hovercar->GetX();
			oldHovercarZ = hovercar->GetZ();
			frameTimer++;
		}
		else if (frameTimer >= 20)
		{
			frameTimer = 0;
		}

		// Exit the game
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}

		// Camera movement manual
		if (currentCameraState == manual)
		{
			if (myEngine->KeyHeld(Key_Up))
				myCamera->MoveZ(cameraSpeed * kGameSpeed * frameTime);

			if (myEngine->KeyHeld(Key_Down))
				myCamera->MoveZ(-cameraSpeed * kGameSpeed * frameTime);

			if (myEngine->KeyHeld(Key_Left))
				myCamera->MoveX(-cameraSpeed * kGameSpeed * frameTime);

			if (myEngine->KeyHeld(Key_Right))
				myCamera->MoveX(cameraSpeed * kGameSpeed * frameTime);

			//Reset camera position
			if (myEngine->KeyHit(Key_1))
			{
				ResetCameraPosition(myEngine, myCamera);
				thirdPerson = true;
			}

			// Switching to first person
			if (myEngine->KeyHit(Key_2))
			{
				myCamera->SetPosition(hovercarXPosition, 5, hovercarZPosition + 5);
				thirdPerson = false;
			}
		}

		// Camera movement automatic
		if (currentCameraState == automatic)
		{
			//Reset camera position
			if (myEngine->KeyHit(Key_1))
			{
				thirdPerson = true;
			}

			// Switching to first person
			if (myEngine->KeyHit(Key_2))
			{
				thirdPerson = false;
			}

			if(thirdPerson == true)
				myCamera->SetPosition(0, 12, -22);
			else if(thirdPerson == false)
				myCamera->SetPosition(0, 5, 5);

		}

		//Switch to auto camera 
		if (myEngine->KeyHit(Key_3))
		{
			currentCameraState = automatic;
		}

		// Camera rotation
		float mouseMovement = myEngine->GetMouseWheelMovement();
		if (mouseMovement > 0.0f && currentCameraRotation > cameraRotationUpLimit)
		{
			myCamera->RotateX(-cameraRotationSpeed * kGameSpeed * frameTime);
			currentCameraRotation -= cameraRotationSpeed * kGameSpeed * frameTime;
		}
		else if (mouseMovement < 0.0f && currentCameraRotation < cameraRotationDownLimit)
		{
			myCamera->RotateX(cameraRotationSpeed * kGameSpeed * frameTime);
			currentCameraRotation += cameraRotationSpeed * kGameSpeed * frameTime;
		}

		// Saving the hovercar positions
		hovercarXPosition = hovercar->GetLocalX();
		hovercarZPosition = hovercar->GetLocalZ();

		// on contact with world edge
		if (hovercar->GetX() > worldEdge || hovercar->GetX() < -worldEdge ||
			hovercar->GetZ() > worldEdge || hovercar->GetZ() < -worldEdge)
		{
			hovercar->SetPosition(hovercarStartingXPos, 0, hovercarStartingZPos);
		}

		// Game start stage
		if (currentGameState == game_start)
		{
			stringstream gameStateDisplay;
			gameStateDisplay << "Game State =" << currentGameState;
			myFont->Draw(gameStateDisplay.str(), gameStateXPosition, gameStateZPosition);
			gameStateDisplay.str(""); // Clear text

			myFont->Draw("Hit Space to Start", 550, 280);
		}

		// Transitioning to screen prompt stage
		if (myEngine->KeyHit(Key_Space) && currentGameState == game_start)
		{
			currentGameState = screen_prompt;
		}

		// Screen prompt stage
		if (currentGameState == screen_prompt)
		{
			stringstream outText;
			outText << "Game State =" << currentGameState;
			myFont->Draw(outText.str(), gameStateXPosition, gameStateZPosition);
			outText.str(""); // Clear text


			//Timer 3 to timer 2
			if (timer < 1 && currentTimer == timer_3)
			{
				myFont->Draw("3", 630, 280);
				timer += 1 * kGameSpeed * frameTime;
			}
			else if (timer >= 1 && currentTimer == timer_3)
			{
				currentTimer = timer_2;
				timer = 0;
			}

			//Timer 2 to timer 1
			if (timer < 1 && currentTimer == timer_2)
			{
				myFont->Draw("2", 630, 280);
				timer += 1 * kGameSpeed * frameTime;
			}
			else if (timer >= 1 && currentTimer == timer_2)
			{
				currentTimer = timer_1;
				timer = 0;
			}

			//Timer 1 to timer go
			if (timer < 1 && currentTimer == timer_1)
			{
				myFont->Draw("1", 630, 280);
				timer += 1 * kGameSpeed * frameTime;
			}
			else if (timer >= 1 && currentTimer == timer_1)
			{
				currentTimer = timer_go;
				timer = 0;
			}

			//Timer go to stage 1
			if (timer < 1 && currentTimer == timer_go)
			{
				myFont->Draw("Go!", 615, 280);
				timer += 1 * kGameSpeed * frameTime;
			}
			else if (timer >= 1 && currentTimer == timer_go)
			{
				currentGameState = stage1;
				timer = 0;
				currentTimer = timer_stage1_complete;
			}

		}

        // Valid for stages 1-5
		if (currentGameState == stage1 || currentGameState == stage2 || currentGameState == stage3 || currentGameState == stage4 || currentGameState == stage5)
		{

			// Get the facing vector

			hovercar->GetMatrix(&matrix[0][0]);
			vector2D facingVector = { matrix[2][0], matrix[2][2] };

			// Calculate thrust (keyboard input)
			if (myEngine->KeyHeld(Key_A))
			{
				hovercar->RotateY(-hovercarTurningSpeed * kGameSpeed * frameTime);
				currentHovercarRotation -= hovercarTurningSpeed * kGameSpeed * frameTime;

			}

			if (myEngine->KeyHeld(Key_D))
			{
				hovercar->RotateY(hovercarTurningSpeed * kGameSpeed * frameTime);
				currentHovercarRotation += hovercarTurningSpeed * kGameSpeed * frameTime;

			}

			if (myEngine->KeyHit(Key_W))
			{
				lastKeyHit = W;
			}
			if (myEngine->KeyHit(Key_S))
			{
				lastKeyHit = S;
			}

			if (myEngine->KeyHeld(Key_W))
			{
				if (myEngine->KeyHeld(Key_Space))
				{
					if (boostTimer == timer_boost_duration && currentBoostTimer < boostDuration)
					{
						thrust = scalar((hovercarSpeed/2)*3, facingVector);
						currentBoostTimer += 1 * kGameSpeed * frameTime;
					}
					else if (boostTimer == timer_boost_duration && currentBoostTimer >= boostDuration)
					{
						boostTimer == timer_boost_cooldown;
						currentBoostTimer = 0.0f;
					}

				}
				else thrust = scalar(hovercarSpeed, facingVector);
			}
			else if (myEngine->KeyHeld(Key_S))
			{
				if (myEngine->KeyHeld(Key_Space))
				{
					if (boostTimer == timer_boost_duration && currentBoostTimer < boostDuration)
					{
						thrust = scalar(-(hovercarSpeed / 4) * 3, facingVector);
						currentBoostTimer += 1 * kGameSpeed * frameTime;
					}
					else if (boostTimer == timer_boost_duration && currentBoostTimer >= boostDuration)
					{
						boostTimer == timer_boost_cooldown;
						currentBoostTimer = 0.0f;
					}
				}
				else thrust = scalar(-hovercarSpeed/2, facingVector);
		    }
			else
				thrust = { 0.0f, 0.0f };


			// Disable boost if below 30 hp
			if (hovercarCurrentHealth < 30)
			{
				currentBoostTimer = boost_unavailable;
			}

			// Calculate drag ( based on previous momentum )
			if (boostTimer == timer_boost_cooldown && currentBoostTimer < boostCooldown)
			{
				drag = scalar(dragCoefficient * 2, momentum);
				currentBoostTimer += kGameSpeed * frameTime;
			}
			else if (boostTimer == timer_boost_cooldown && currentBoostTimer >= boostCooldown)
			{
				boostTimer = timer_boost_duration;
				currentBoostTimer = 0.0f;
			}
			else drag = scalar(dragCoefficient, momentum);

			// Calculate momentum ( based on thrust, drag and momentum )

			momentum = sum3(momentum, thrust, drag);


			// Move the hovercar

			hovercar->Move(momentum.x, 0.0f, momentum.z);

			// Hovercar Speed in km/h
			hovercar_metres_seconds = (sqrt(momentum.x*momentum.x + momentum.z*momentum.z)) * 100 * gameScale;

			hovercar_kilometres_hour = 3.6 * hovercar_metres_seconds;


			//Displaying the game state
			stringstream gameStageText;
			gameStageText << "Game State =" << currentGameState;
			myFont->Draw(gameStageText.str(), gameStateXPosition, gameStateZPosition);
			gameStageText.str(""); // Clear text

			//Displaying the hovercar health
			stringstream hovercarHealthDisplay;
			hovercarHealthDisplay << "Hovercar health: " << hovercarCurrentHealth;
			myFont->Draw(hovercarHealthDisplay.str(), 850, 675);
			hovercarHealthDisplay.str(""); // Clear text

			//Displaying the hovercar speed
			stringstream hovercarSpeed;
			hovercarSpeed << "Hovercar speed: " << hovercar_kilometres_hour << " km/h";
			myFont->Draw(hovercarSpeed.str(), 500, 675);
			hovercarSpeed.str(""); // Clear text

		}

		// Collisions
		if (lastKeyHit == W)
		{
			// Check for collisions ( hovercar to checkpoint walls )
			for (int i = 0; i <= checkpointNumber; i++)
			{
				if (sphere2sphere(hovercarXPosition, hovercarZPosition, hovercarRadius, checkpointWallXPositions[i], checkpointWallZPositions[i], checkpointWallRadius))
				{
					// Collision resolution
					hovercarCurrentHealth--;
					hovercar->MoveLocal(0, 0, -1);
				}
			}

			// Check for collisions ( to walls )
			for (int i = 0; i <= wallNumber; i++)
			{
				if (sphere2box(hovercarXPosition, hovercarZPosition, hovercarRadius, wallXPositions[i], wallZPositions[i], wallWidth, wallDepth))
				{
					// Collision resolution
					hovercarCurrentHealth--;
					hovercar->MoveLocal(0, 0, -1);
				}
			}

			// Check for collisions ( hovercar to water tank_1s )
			for (int i = 0; i <= waterTank1Number; i++)
			{
				if (sphere2sphere(hovercarXPosition, hovercarZPosition, hovercarRadius, waterTank1XPositions[i], waterTank1ZPositions[i], waterTankRadius))
				{
					// Collision resolution
					hovercarCurrentHealth--;
					hovercar->MoveLocal(0, 0, -1);
				}
			}

			// Check for collisions with the water water tank obstacle
			if (sphere2sphere(hovercarXPosition, hovercarZPosition, hovercarRadius,
				waterTankObstacleXPosition, waterTankObstacleZPosition, waterTankRadius))
			{
				// Collision resolution
				hovercarCurrentHealth--;
				hovercar->MoveLocal(0, 0, -1);
			}
		}

		if (lastKeyHit == S)
		{
			// Check for collisions ( hovercar to checkpoint walls )
			for (int i = 0; i <= checkpointNumber; i++)
			{
				if (sphere2sphere(hovercarXPosition, hovercarZPosition, hovercarRadius, checkpointWallXPositions[i], checkpointWallZPositions[i], checkpointWallRadius))
				{
					// Collision resolution
					hovercarCurrentHealth--;
					hovercar->MoveLocal(0, 0, 1);
				}
			}

			// Check for collisions ( to walls )
			for (int i = 0; i <= wallNumber; i++)
			{
				if (sphere2box(hovercarXPosition, hovercarZPosition, hovercarRadius, wallXPositions[i], wallZPositions[i], wallWidth, wallDepth))
				{
					// Collision resolution
					hovercarCurrentHealth--;
					hovercar->MoveLocal(0, 0, 1);
				}
			}

			// Check for collisions ( hovercar to water tank_1s )
			for (int i = 0; i <= waterTank1Number; i++)
			{
				if (sphere2sphere(hovercarXPosition, hovercarZPosition, hovercarRadius, waterTank1XPositions[i], waterTank1ZPositions[i], waterTankRadius))
				{
					// Collision resolution
					hovercarCurrentHealth--;
					hovercar->MoveLocal(0, 0, 1);
				}
			}

			// Check for collisions with the water water tank obstacle
			if (sphere2sphere(hovercarXPosition, hovercarZPosition, hovercarRadius,
				waterTankObstacleXPosition, waterTankObstacleZPosition, waterTankRadius))
			{
				// Collision resolution
				hovercarCurrentHealth--;
				hovercar->MoveLocal(0, 0, 1);
			}
		}
		
		

        // Stage 1
		if (currentGameState == stage1)
		{
			// Transitioning to stage 2
			if (point2box(hovercarXPosition, hovercarZPosition, checkpointXPositions[0], checkpointZPositions[0], checkpointClearWidth, 1))
				currentGameState = stage2;
		}

		// Stage 2
		if (currentGameState == stage2)
		{
			if (timer < 1 && currentTimer == timer_stage1_complete)
			{
				myFont->Draw("Stage 1 Complete", 550, 300);
				timer += 1 * kGameSpeed * frameTime;
			}
			else if (timer >= 1 && currentTimer == timer_stage1_complete)
			{
				timer = 0;
				currentTimer = timer_stage2_complete;
			}

			// Transitioning to stage 3
			if (point2box(hovercarXPosition, hovercarZPosition, checkpointXPositions[1], checkpointZPositions[1], 1, checkpointClearWidth))
				currentGameState = stage3;
		}

		// Stage 3
		if (currentGameState == stage3)
		{
			if (timer < 1 && currentTimer == timer_stage2_complete)
			{
				myFont->Draw("Stage 2 Complete", 550, 300);
				timer += 1 * kGameSpeed * frameTime;
			}
			else if (timer >= 1 && currentTimer == timer_stage2_complete)
			{
				timer = 0;
				currentTimer = timer_stage3_complete;
			}

			// Transitioning to stage 3
			if (point2box(hovercarXPosition, hovercarZPosition, checkpointXPositions[2], checkpointZPositions[2], checkpointClearWidth, 1))
				currentGameState = stage4;
		}

		// Stage 4
		if (currentGameState == stage4)
		{
			if (timer < 1 && currentTimer == timer_stage3_complete)
			{
				myFont->Draw("Stage 3 Complete", 550, 300);
				timer += 1 * kGameSpeed * frameTime;
			}
			else if (timer >= 1 && currentTimer == timer_stage3_complete)
			{
				timer = 0;
				currentTimer = timer_stage4_complete;
			}

			// Transitioning to stage 3
			if (point2box(hovercarXPosition, hovercarZPosition, checkpointXPositions[3], checkpointZPositions[3], checkpointClearWidth, 1))
				currentGameState = stage5;
		}

		// Stage 5
		if (currentGameState == stage5)
		{
			if (timer < 1 && currentTimer == timer_stage4_complete)
			{
				myFont->Draw("Stage 4 Complete", 550, 300);
				timer += 1 * kGameSpeed * frameTime;
			}
			else if (timer >= 1 && currentTimer == timer_stage4_complete)
			{
				timer = 0;
				currentTimer = timer_stage5_complete;
			}

			// Transitioning to stage 3
			if (point2box(hovercarXPosition, hovercarZPosition, checkpointXPositions[4], checkpointZPositions[4], checkpointClearWidth, 1))
				currentGameState = race_finished;
		}

		// Race finished
		if (currentGameState == race_finished)
		{
			//Displaying the game state
			stringstream gameStageText;
			gameStageText << "Game State =" << currentGameState;
			myFont->Draw(gameStageText.str(), gameStateXPosition, gameStateZPosition);
			gameStageText.str(""); // Clear text

			//Displaying the hovercar health
			stringstream hovercarHealthDisplay;
			hovercarHealthDisplay << "Hovercar health: " << hovercarCurrentHealth;
			myFont->Draw(hovercarHealthDisplay.str(), 850, 675);
			hovercarHealthDisplay.str(""); // Clear text

			myFont->Draw("Race Finished!", 550, 300);
		}

		// Game over on 0 hp
		if (hovercarCurrentHealth <= 0)
		{
			currentGameState = game_over;
		}

		// Game over
		if (currentGameState == game_over)
		{
			//Displaying the game state
			stringstream gameStageText;
			gameStageText << "Game State =" << currentGameState;
			myFont->Draw(gameStageText.str(), gameStateXPosition, gameStateZPosition);
			gameStageText.str(""); // Clear text

			//Displaying the hovercar health
			stringstream hovercarHealthDisplay;
			hovercarHealthDisplay << "Hovercar health: " << hovercarCurrentHealth;
			myFont->Draw(hovercarHealthDisplay.str(), 850, 675);
			hovercarHealthDisplay.str(""); // Clear text

			//Displaying game over message
			myFont->Draw("Game over!", 580, 300);

			hovercarCurrentHealth = 0;
		}


	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}

//Function definitions
void ResetCameraPosition(I3DEngine* &myEngine, ICamera* &myCamera)
{
	myCamera->SetPosition(0, 12, -72);
	myCamera->ResetOrientation();
	myCamera->RotateX(18);
}

bool sphere2sphere(float sphere1xPosition, float sphere1zPosition, float sphere1Radius,
				   float sphere2xPosition, float sphere2zPosition, float sphere2Radius)
{
	float xDistance = sphere2xPosition - sphere1xPosition;
	float zDistance = sphere2zPosition - sphere1zPosition;
	float distance = sqrt(xDistance * xDistance + zDistance * zDistance);

	return (distance < (sphere1Radius + sphere2Radius)); // If all are true then the expression is true
}

bool sphere2box(float spherexPosition, float spherezPosition, float sphereRadius,
				float boxxPosition,    float boxzPosition,    float boxWidth,	 float boxDepth)
{
	float minX = boxxPosition - boxWidth / 2 - sphereRadius;
	float maxX = boxxPosition + boxWidth / 2 + sphereRadius;
	float minZ = boxzPosition - boxDepth / 2 - sphereRadius;
	float maxZ = boxzPosition + boxDepth / 2 + sphereRadius;

	return(spherexPosition > minX && spherexPosition < maxX && spherezPosition > minZ && spherezPosition < maxZ);
}

bool point2box(float pointXPosition, float pointZPosition, float boxXPosition, float boxZPosition, float boxWidth, float boxDepth)
{
	float minX = boxXPosition - boxWidth / 2;
	float maxX = boxXPosition + boxWidth / 2;
	float minZ = boxZPosition - boxDepth / 2;
	float maxZ = boxZPosition + boxDepth / 2;

	return(pointXPosition > minX && pointXPosition < maxX && pointZPosition > minZ && pointZPosition < maxZ);
}
