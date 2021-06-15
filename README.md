# JimmyGod Engine
![This is main picture](https://github.com/TheJimmyGod/JimmyGod_Engine/blob/master/Images/JimmyGod-Engine.png)
### Contents
1. [Introduction](https://github.com/TheJimmyGod/JimmyGod_Engine#introduction)
2. [Guide to use JimmyGod Engine](https://github.com/TheJimmyGod/JimmyGod_Engine#guide-to-use-jimmygod-engine)
3. [Features](https://github.com/TheJimmyGod/JimmyGod_Engine#features)
4. [Examples](https://github.com/TheJimmyGod/JimmyGod_Engine#examples-1)
5. [External Libraries](https://github.com/TheJimmyGod/JimmyGod_Engine#external-libraries)
6. [License](https://github.com/TheJimmyGod/JimmyGod_Engine#license)
7. [Acknowledgements](https://github.com/TheJimmyGod/JimmyGod_Engine#acknowledgement)

### **Introduction**
**JimmyGod Engine** is the custom game engine in C++ language with Direct3D rendering. The name JimmyGod comes from my vision that it will be a high quality and popular game engine. The game engine was started in November 2019, and continues to be updated and improved. The game engine consists of various libraries:*Input, Math, Graphics, AI, Machine Learning, and Physics*. **The input library** has support for mouse and keyboard. **The graphics library** allows to easily use GPU shaders to render 3D models with support for dynamic lighting, shadows, post processing, and animation. **The math library** has support for 3D geometry and linear algebra components. Moving on, **the AI library** has pathfinding, steering, finite-state-machine for simulating autonomous agents. For **the machine learning library**, I implemented K-means, Genetic Algorithm, Neural network, and linear regression to solve various problems. For the physics part, it includes a Verlet particle system with constraints to model object dynamics.

### **Guide to use JimmyGod Engine**

##### **Requirement**
###### Visual studio 2019
###### Latest SDK version

##### **Steps**
##### 1. Firstly, you should check your SDK version and Visual Studio version.
##### 2. Secondly, you should link the main library for your project.
###### **Examples:**
```C++
#include <JimmyGodEngine/Inc/JimmyGodEngine.h>
```
##### 3. Optionally, you can add Imgui components in your class
```C++
#include <ImGui/Inc/imgui.h>
```
### **Features**
![This is lists](https://github.com/TheJimmyGod/JimmyGod_Engine/blob/master/Images/Classcify.PNG)
#### *Artificial Intelligence(AI)*
- Implemented ```AI world``` helps to link one or many agents to AI cores in gameplay
- Implemented ```Pathfinding System``` supports to explore a way exactly for agents:
  - ```Breadth First Search(BFS)```
  - ```Depth First Search(DFS)```
  - ```Dijkstra```
  - ```A*```
- Implemented ```Finite State Machine``` supports to include a lot of behaviors
- Implemented ```Steering System``` supports one or many agents to move the way point smoothly:
  - ```Seek```
  - ```Flee```
  - ```Arrive```
  - ```Pursuit```
  - ```Evade```
  - ```Wander```
  - ```Hide```
  - ```Obstacle Avoidance```
  - ```Wall Avoidance```
  - ```Alignment```
  - ```Cohesion```
  - ```Separation```
- Implemented ```Perception System``` supports agents to detect nearby neighbors in a sight of ```Visual Sensor```
 #### *Core*
- Implemented ```Core System``` helps to launch windows application with Win32 API
- Implemented ```Debug``` and ```Utility for Time```
- Implemented Memory managements:
  - ```Block Allocator```: Support to store and release a chunk of memory of single or many objects in order to prevent memory fragmentation
  - ```Typed Allocator```: Support to be used to a specific type within ```Block Allocator```
  - ```Handle```: This is a pool that manages the accessibility of a memory and registers the validation of handles
- Implemented ```the Meta System``` to usefully retrieve objects information in run time:
  - ```ARRAY```
  - ```CLASS```
  - ```FIELD```
  - ```POINTER```
#### *Graphics*
- Implemented ```Grapics System``` helps to utilize Direct3D rendering supported
- Implemented ```Model Importer``` supports to save and load a model
- Implemented ```Animation``` handles one or many object's transform for each frame
- Implemented ```Model``` and ```Skeleton``` support to render textures and motions with 3D
- Implemented ```Post Processing``` allows to render the screen has new effects with ```HLSL```
- Implemented ```SimpleDraw``` supports to create various shapes with Debug UI:
  - ```Line```
  - ```Cube```
  - ```Sphere```
  - ```AABB```
  - ```OBB```
  - ```Displacement```
  - ```Plane```
- Implemented ```Camera``` allows to watch around and oriented into world view
- Implemented ```Matrials``` offers various utility to render an object:
  - ```Textures```
  - ```Colors```
  - ```Matrial```
  - ```Sampler```
  - ```LightTypes```
  - ```VertexTypes
- Implemented ```MeshBuilder``` supports to render a normal or textured mesh with:
  - ```Cube``` 
  - ```Cylinder```
  - ```Plane```
  - ```Sphere```
  - ```Quad```
- Implemented ```TextureManager``` and ```SpriteRenderManager``` guide to render each texture and sprite
#### *Input*
- Implemented ```Input System``` allows to use keyborad and mouse in game world
#### *Machine Learning*
- Implemented ```Machine Learning System``` allows to use machine learing features
- Implemented ```Linear Regression``` and ```Logistic Regression``` support to track or predict between two variables or factors
- Implemented ```K-means``` supports to explore groups which have not been explicitly labeled in the data
- Implemented ```Genetic Algorithm``` supports to solve both constrained and unconstrained optimization problems based on a natural selection process that mimics biological evolution
#### *Math*
- Implemented ```Linear Algebra``` components:
  - ```Vector2```
  - ```Vector3```
  - ```Vector4```
  - ```Matrix3```
  - ```Matrix4```
  - ```Quaternion```
- Implemented ```Geometry``` components:
  - ```AABB```
  - ```OBB```
  - ```Ray```
  - ```Sphere```
  - ```Circle```
  - ```Line Segment```
  - ```Rectangle```
  - ```Plane```
- Implemented ```EngineMath``` supports to all functionally mathmatics need for Linear Algebra and Geometry
#### *Physics*
- Implemented ```Physics World``` allows to handle all physics particles and constraint in game world
- Implemented ```Particle System``` supports to use verlet integration
- Implemented ```Contraint``` enables to apply fixed or spring 
### **Examples**
#### AI
![image of animated picture](https://github.com/TheJimmyGod/JimmyGod_Engine/blob/master/Images/AI_project235511615830.gif)

The AI project is based on the Steering System, acting various steering behaviors, and they would collide obstacles and walls.
#### Graphics
![image of animated picture](https://github.com/TheJimmyGod/JimmyGod_Engine/blob/master/Images/Earth235012680350.gif)

The Graphics project is based on the Solar System, rendering lights and textures for each planet, and post processing would show up the entire screen.
#### Physics and Graphics
![image of animated picture](https://github.com/TheJimmyGod/JimmyGod_Engine/blob/master/Images/Batman236217209735.gif)

The project is based on Batman the super hero, displaying various animations by toggling buttons, and would utilize verlet integration as weapons.

### **External Libraries**
- [Assimp](https://github.com/assimp/assimp) for importing and extracting models
- [ImGui](https://github.com/ocornut/imgui) for Debug User Interface
- [DirectXTK](https://github.com/microsoft/DirectXTK) for Direct tool kit in Direct3D 11
- [FW1FontWrapper](https://github.com/gamelaster/FW1FontWrapper) for rendering fonts in Direct3D 11
- [RapidJson](https://github.com/Tencent/rapidjson/) for serializing and deserializing Json files


### **License**
- Copylightⓒ 2021 JimmyGod [LICENSE](https://github.com/TheJimmyGod/JimmyGod_Engine/blob/master/LICENSE)
### **Acknowledgement**
- Peter Chan
