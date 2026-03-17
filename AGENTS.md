# AGENTS.md

## Basic Rules
- Always respond **briefly and to the point**
- Always respond **in English only**
- Do **not create commits** in the repository unless explicitly requested
- If I ask you to commit changes, after committing you must **push the changes to the repository**
- Do **not run builds automatically**
- Do **not upload firmware automatically**
- Try to keep files **no longer than 200 lines whenever possible**
- Use the **indexed code base** to navigate and understand the project
- Development tasks are located in the `tasks/new` directory
- After completing a task, move the task file to the `tasks/done` directory
- Hardware configuration is located in the `hardware` directory
- The list of hardware components is stored in `hardware/components.md`
- The hardware wiring diagram is stored in `hardware/scheme.md`
- If something **non-standard** needs to be implemented, ask me first
- If you **do not know how to implement something**, ask me first
- The device functionality requirements are located in the folder `requirements`
- Do not run the project build without instructions
- Don't use additional scripts that need to be run before compiling the code
- Use the architecture to process processes in parallel without blocking the entire processor

## Coding Rules
- Development language: **C++ Arduino (PlatformIO)**
- Avoid excessive use of **STL**
- Avoid **dynamic memory allocation**
- The code should be located in the `src` folder
- Before you do this, read what files already contain the code.
- Code must be **modular**
- HTML pages must be placed in **separate `.html` files**
- Remove dead code
- Use a clean architecture approach
- Don't make large code files
- Don't make large functions

## Web Interface Rules
- Use native elements
- Don't use custom styling unless necessary
- The code should be as compact as possible
- The web interface requirements are described in folder `requirements\web` in Wireframe notation
- Do not combine the code for pages into one file