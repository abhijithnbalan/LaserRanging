LASER RANGING
-----------------    
-----------------   
                                    
<div style="text-align: right"> Planys Technologies </div>

### Image Processing based Laser Ranging is a technology which measures distance to an object by projecting (preferrably two) laser dots on the object and capturing its image using a camera.

### Pixel distance from the image is then mapped to actual distance using parallax method.

## Program Flow
--------------------------
![Optional Text](documentation/LaserRanging_flowchart.png)


1. Region of Interest selection

    * The set Region of interest is cropped out of input image for further processing.
   
    * Region can be manually set.

2. Dehazing
    * A quick dehazing can be done for better result in laser ranging. But dehazing can be computationally expensive.
    * CLAHE (Contrast Limited Adaptive Histogram Equilization) Algorithm is used with default clip limit of 2.
    * Also Histogram Equilization is provided for increasing the execution speed with a slight degradation in dehazing quality

3. Color segmentation
    * Colour segementation is done through HSV(Hue Saturation Value) conversion.HSV format extract color from light and white and hence make it easier for colour identification.(Red Hue value range :0-7 and 170-180)
    * To extract white dot in the middle of laser, HSL(Hue, Saturation LIghtness) conversion is used. White is filtered in as highest values of Lightness channel
    * Threshold can be manually set for better performance.
4. Contour identification
    * Contours are identified from segmented image and bounding rectangles are drawn.
    * Center of laser dot is approximated as the center of bounding rectangle drawn.
    * Lines are drawn connecting centers and pixel distance is also calculated.
5. Overlay Data
    * Some of the Data are direclty overlayed to the output frame.
    * Pixel distances are mapped to actual distance using parallax method.

6. Preview Data
    * Two modes are given for preview of data. Single preview and multiple preview mode.
    * Multiple Preview mode can be used to track the process by viewing the image at different stages of processing.

## Laser Ranging modes
---------------------------

### 1. Single laser mode
Single laser mode will provide two set of distances corresponding to two lasers. This mode can measure distance even if one laser is present (either one dot is visible of technical problems in the other laser). Since it gives two different distance measurement, angle of target plane can also be found out form single frame. 

However, efficient working of sinle laser mode requires proper calibration to find out the reference point from which distance to each points will be measured.

### 2. Dual laser mode
Dual laser mode requires two laser dots to be identified simultaneously for distance measurement. Angle of target plane can be found out in distance data from multiple frames.

Dual laser mode does not need calibration.

### 3. Laser calibration mode
Laser calibration is only needed for single laser mode for getting the center of lasers and also the parallax constant for finding the distance. There is a seperate executable given for calibration.

**Calibration process** : Bring a plate infront of ROV at a specific distance from the camera and position it such a way that the plate is normal to camera. The colour of the plate should be choosen such that laser detection is easier. The distance chosen has to be recorded in the program. Not doing so will make the program re use the distance fixed last time. Execute laser ranging and when two laser dots are identified, the corresponding centers are captured and also the parallax constant. Laser center will be the mid-point of the line connecting centers of individual laser dots.
The mid-point is found out and written to a local file.

After Calibration, a local file will be updated with the latest laser center values, the distance used in calibration and the parallax constant. This data will be used for further laser ranging. 

## Parallax Method
---------------
Parallax method is a widely used method in astronomy. It uses angular difference betwee two image captured of a same object but from different positions. Given the distance between the positions of two pictures, distance from object to camera(mid point of line connecting two camera positions) can be found out.

According to parallax method, the actual distance is inversely proportional to the pixel distance and the proportionality constant (called parallax constant in program) is found out at the time of calibration. This value does not change unless Camera, camera position or laser positions are changed.

## Using the Program
-------------------------



### Modes of running the program

1. Execution mode

No display and other controls. Just console output is present. No loops present

```
./LaserRanging  exe <image file (cv::Mat)>
```
This runs for one image and terminates. Image has to be shared variable and the program will be running multiple times



2. Developer mode

Allows control and preview. Loop is present

```
./LaserRanging  dev <video/image file path / camer device number>
```


### Laser Ranging

```
cd bin
./LaserRanging <mode>
```

### Laser Calibration


```
cd bin
./LaserCalibration <mode>
```


Detailed readme file can be accessed [here](readme.md)