# Navigation example project
----
## Requirements
- Qt5
- VTK6
- ITK4

----
## Modules
### 1. MainWindow
This is full example illustrating how the project work. 

**Functions** 

   - Load a 3d image (.nii format)
   - Orthogonal View
   - MarchingCubes to create a 3D model
   - Configure 2 kinds of tracking devices
   - Landmark Registration
   - ICP registration

### 2. Calibration Tool(Submodule of MainWindow)

### 3. PivotCalibration

### 4. VisualizationUtil
Custom visualization module. This module is for private. Free to delete.

### 5. PolarisVicra
PolarisVicra API

### 6. ATC3DG
ATC3DG: trackStar API


### 7. vtkTrackerUtil
vtkTrackerUtil is a base module:

- **utilities**:

Quaternion, 2D array, vtkMatrix4x4 conversions.

- **Configuration Parser**

- **MarkCapture**:

Real world position capture

- **Registration**:

Landmark registration; ICP registration


----
## Usage

- Use cmake to configure the project
- Modules for tracking are nested so you should take them as a whole.
- All the *_test folders are examples for testing. They can be deleted.
- IP_check is also useless







