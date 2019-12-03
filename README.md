![date](https://img.shields.io/badge/Build%20Date-26.11.19-informational)
# OpenGL Rendering Showcase ~ Alien Invasion 👽
A showcase of rendering techniques using OpenGL, intially created to showcase the Exalted rendering engine, and then extended as an exercise for the CSC8503 graphics module at Newcastle University.

This repo uses *Exalted*, a custom engine I developed as a learning exercise with the purposes of learning engine design and architecture.

Youtube Video: https://www.youtube.com/watch?v=IS9GLXPMrL8&feature=youtu.be

<a name="list-of-features"></a>
## List of Features
[active]:https://img.shields.io/badge/-Active-success
[depreciated]:https://img.shields.io/badge/-Depreciated-inactive
[updating]:https://img.shields.io/badge/-Updating-purple
[passing]:https://img.shields.io/badge/-Passing-success
[outdated]:https://img.shields.io/badge/-Outdated-blue
[dev]:https://img.shields.io/badge/-Unreleased-important

[version-1.0.0]:https://img.shields.io/badge/Ver.-1.0.0-brightgreen
[version-1.0.1]:https://img.shields.io/badge/Ver.-1.0.1-ff69b4
[version-1.0.2]:https://img.shields.io/badge/Ver.-1.0.2-ff69b4
[version-1.0.3]:https://img.shields.io/badge/Ver.-1.0.3-ff69b4
[version-1.0.4]:https://img.shields.io/badge/Ver.-1.0.4-ff69b4
[version-1.0.5]:https://img.shields.io/badge/Ver.-1.0.5-ff69b4
[version-dev]:https://img.shields.io/badge/Ver.-dev-important

|**Name**|**Description**|
|:------:|:-------------:|
| Scene Graph| Used to arrange spatial data in the scenes, for example, to allow for blended transparency ordering |![version-1.0.0][version-1.0.0]|
| Blinn-Phong Lighting (Energy Conserving) | (Directional - Spot - Point) lighting |![version-1.0.0][version-1.0.0]|
| Environment Mapping | Skybox environment |![version-1.0.0][version-1.0.0]|
| Dynamicly Changing Environment | Objects in the scenes change over time |![version-1.0.0][version-1.0.0]|
| Real-Time Soft Shadows | Spot (perspective), Point (cubemapped perspective) and directional (orthographic) multi-sampled shadows |![version-1.0.0][version-1.0.0]|
| Multi-Viewport Shadowmap Debug Displays | Displays normalized shadowmaps as mini-display views |![version-1.0.0][version-1.0.0]|
| Post Processing | Color Inversion - Grayscale - Sharpening - Blurring - Edge Detection |![version-1.0.0][version-1.0.0]|
| Anisotropic Filtering | 16x anisotropic texture filtering |![version-1.0.0][version-1.0.0]|
| Object Instancing | Instance drawing rendering capabilities, can be used for particle systems etc... |![version-1.0.0][version-1.0.0]|

#### Some Screenshots 

#### Showcase Scene
A screenshot extracted from [this](https://www.youtube.com/watch?v=IS9GLXPMrL8&feature=youtu.be) youtube video of the showcase:
<a name="showcase"></a>
<p align="center"><img width=100% src="Screenshots/Geometry_Explosion.jpg?raw=true"></p>

##### Omnidirectional Perspective Shadows
Omnidirectional real-time Shadows: 
[![omnidirectional-shadows](https://i.gyazo.com/b1e98fb9e85f1e247d8dfcb3449b9a7a.gif)](https://gyazo.com/b1e98fb9e85f1e247d8dfcb3449b9a7a)

##### Orthographic Shadows
Dynamic directional real-time shadows: 
[![Dynamic-Directional-Shadows](https://i.gyazo.com/2b6d543d7c08773d9b316df537f93611.gif)](https://gyazo.com/2b6d543d7c08773d9b316df537f93611)

##### Instanced Rendering
10 million quads rendered at 150fps:
<a name="anisotropic filtering"></a>
<p align="center"><img width=100% src="Resources/Images/Instancing/10Mil_Objects_150fps.jpg?raw=true"></p>

##### Scene Graph
Scene graph object transformation as done in Unity: 
<a name="scene-graph"></a>
<p align="center"><img width=100% src="Resources/Images/Scene-Graph/Hierarchy-Unity-Comparison-Successfulpng.png?raw=true"></p>

<a name="github-issues"></a>
## Github Issues
Please use [Github Issues](https://github.com/Kney-Delach/OpenGL-Rendering/issues) to report bugs.
