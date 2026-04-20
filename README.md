# Scarlet Project
 
A custom C++ game engine using OpenGL for rendering and a custom implementation of ECS - a hybrid of Sparse-Set and Archetype.

## Requirements 
 - Visual Studio 2022 (2026 can also be used)
 - Python (installed to path so commands can be run from terminal/command prompt)

## Setup

 - Clone repository
 - Run `GenerateSolution.bat`

This will generate a visual studio sln file.

Open `Scarlet.sln`. The solution can then be built in `Dev` and `Release` configuration.

Dev Configuration contains the editor UI.
Release Configuration builds the projects that will be used for runtime builds.

Note the Scarlet solution shouldn't be run in Release mode, but rather just built as the built files are used in the game builds and linking.

If developing whilst creating a project, `Scarlet` project can be set as main target, and should be run wihtout debugging in Visual Studio to ensure that hot-reloading works.
If run with debugging symbols, Visual Studio will lock the pdb files, preventing the ability to build the projects.
