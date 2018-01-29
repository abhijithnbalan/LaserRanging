
LASER RANGING
-----------------    
-----------------   
                                    
<div style="text-align: right"> Planys Technologies </div>

### Image Processing based Laser Ranging is a Technology which measures distance to an object by projecting (two preferrably) laser dots on the object and capturing its image using a camera.



## SETIING UP
Make sure you have the following files in your system
1. main.cpp
2. laser_ranging.cpp
3. laser_ranging.h
4. image_processing.cpp
5. image_processing.h
6. view_frame.cpp
7. view_frame.h
8. capture_frame.cpp
9. capture_frame.h
10. timer.cpp
11. timer.h
12. algorithm.cpp
13. algorithm.h
14. laser\_calibration_values.json
15. CMakeLists.txt

Now to compile, run the following commands on terminal

```
cmake .
make
```
The code offers multiple modes of execution

1. devoloper mode:

    This is the complete model. every outout will be shown and all the functionalities will be avalilable.
    disabling this mode will disable every displays functions.
        
    ```c++
    Ranger.dev_mode = true or false;
    or
    dev_mode = true or false;//on main
    ```

2. execution mode:

        This mode is the minimal version and right now only working with image an image stream directly obtained from vision.

    ```c++
    exe_mode = true or false;//on main
    ```


To run the code,
1. Laser Ranging with Video file.
```
./LaserRanging <path to video>
```

2. Laser Ranging with Image file.
```
./LaserRanging <path to image>
```

3. Laser Ranging with camera input
```
./LaserRanging <camera device number>
```

4. Laser Ranging without arguments
```
./LaserRanging
```
The code loads a default video file "rendered.mp4"

## RUNNING THE CODE

*(for the following section, Ranger is used as an object to the class LaserRanging)

### Public Variables that can be set manually

1. Ranger.laser\_range\_status

   ` true` : Laser ranging will be activaed

   `false` : Laser ranging will be de-activated and direct feed from camera/video will be given back

2. Ranger.use\_dynamic\_control

    `true` : Control Panel GUI will be available for dynamic parameter changeing.

    `false` : Control Panel become unavailable.


3. Ranger.use_white

    `true` : Filters white colors also for color segmentation (useful when laser dots become white instead of a color)

    `false` : Only filters set color.

4. Ranger.use_dehaze

    `true` : The frames(Region of interset only) will be dehazed using basic dehazing algorithms before processing. Gives better results but computationally expensive.

    `false` : Process images without dehazing

5. Ranger.calibration_status

    `true` : Calibration for laser get executed and the main program resumes after calibration.

    `false` : Skips calibration.

6. Ranger.calib_trigger

    `true` : Trigger for capturing laser center values from Calibraiton process. Gets set back to false automatically.

    `false` : Calibration continues 

7. Ranger.calib_cancel

    `true` : Cancels Calibration and resumes the main program.

    `false` : Calibration continues

8. Ranger.distance\_between_laser

    ```int```: Stores Actual distance between laser pointers


9. Ranger.laser_center\_x
    
    ```int```: Manually sets x coordinate of laser point's center. 

10. Ranger.laser_center\_y

    ```int```: Manually sets y coordinate of laser point's center. 


### Public functions that can be used set parameters

1. Ranger.set_roi()

    Set region of interst passing x and y coordinate of center point and width and height of the needed region of interst

    ```c++
    Ranger.set_roi(int x, int y, int width, int height);
    ```

2. Ranger.set_threshold()

    Set threshold values for color based image segmentation passing hue,saturation and value limits
     ```c++
    Ranger.set_threshold(int hue_lower_limit, int hue_upper_limit, int saturation_lower_limit, int saturation_upper_limit, int value_lower_limit, int value_upper_limit);
    ```

3. Ranger.algo.set\_CLAHE\_clip\_limit()

    Set clip limit(parameter for CLAHE based dehazing) 
    ```c++
    Ranger.algo.set_CLAHE_clip_limit(int clip_limit)
    ```

### Publically accessible variables

1. Ranger.range_mm

    ```float  ```:  Returns range in millimeter

2. Ranger.range_ll\_mm

    ```float  ```: Returns range according to left in millimeter

3. Ranger.range_rl\_mm

    ```float  ``` : Returns range according to right in millimeter

4. Ranger.original_frame

    ```CaptureFrame``` : Returns CaptureFrame object containing the frame which got passed into the function

5. Ranger.ROI_frame

     ```CaptureFrame```: Returns CaptureFrame object containing the region of interest.

6. Ranger.dehzed_frame

     ```CaptureFrame``` : Returns CaptureFrame object containing the dehazed image.

7. Ranger.segmented_frame

     ```CaptureFrame``` : Returns CaptureFrame object containing the frame which got passed into the function

8. Ranger.contour_overlay\_frame

    ```CaptureFrame``` : Returns CaptureFrame object containing the contour identified. This will be a single channel image.

9. Ranger.line_overlay\_frame

    ```CaptureFrame``` : Returns CaptureFrame object containing the line drawn connecting laser points. This will be a single channel image.

### Publically accessible functions to get data

1. Ranger.get_laser\_disatance\_px()

    To retrieve the distance in pixels between two laser points.
    ```c++
    float distance = Ranger.get_laser_disatance_px()
    ```

2. Ranger.get_left\_laser\_disatance\_px()

    To retrieve the distance in pixels from left laser point to laser center. Used in single laser ranging enebled mode.
    ```c++
    float distance = Ranger.get_left_laser_disatance_px()
    ```

3. Ranger.get_right\_laser\_disatance\_px()

    To retrieve the distance in pixels from right laser point to laser center. Used in single laser ranging enebled mode.
    ```c++
    float distance = Ranger.get_right_laser_disatance_px()
    ```

4. Ranger.scale\_L\_by\_px()

    To retrieve the ratio of distance between lasers to the pixel distance between the laser points. This value is used for UI scale of referance.
    ```c++
    float scale = Ranger.scale_L_by_px()
    ```

5. Ranger.angle_of\_tilt()

    To retrieve the angle of tilt of the surface to which the lasers are pointed. The angle will be in degrees.
    ```c++
    float angle = Ranger.angle_of_tilt()
    ```

## Program Flow
![Optional Text](/LaserRanging.png)


## INCLUDED FILES

   

## 1. laser_ranging.cpp   
            
 ### Class LaserRanging

 Derived class of Class ImageProcessing. Have functions and variables exclusively used for Laser Ranging.

#### Functinos

 * CaptureFrame contour_distance(CaptureFrame)              :
    
    *Identify contours and distance between them in a segmented image*              
 * CaptureFrame contour_distance_single_laser(CaptureFrame)   : 
    
    *Identify contours and distance between them in a segmented image for single laser ranging enabled*

 * float get_laser_distance ()             : 
    
    *Distance from two laser dots*
 * float get_left_laser_distance()     : 
    
    *Distance according to left laser dot*      
 * float get_right_laser_distance ()       : 
    
    *Distance according to right laser dot*  
 *   void laser_ranging (CaptureFrame)                  : 
    
      *Single function to call all associated functions for laser ranging*   
 * void laser_ranging_single_laser (CaptureFrame)     : 
    
    *Single function to call all associated functions for laser ranging with single laser ranging enabled*
 * CaptureFrame LR_data_overlay(CaptureFrame)                   : 
    
    *Show overlays associated with laser ranging*   
 * CaptureFrame LR_data_overlay_single_laser(CaptureFrame)       : 
    
    *Show overlays associated with laser ranging with single laser ranging enabled* 

 ***Public Variables***
 * laser_center_x                  : 
        
    *to change the center point between two lasers (single laser only)*
 * laser_center_y                  : 
        
    *to change the center point between two lasers*
 * range                           : 
        
    *to extract range data*
 * left_range and right_range      : 
        
    *to extract left range data and right range data (single laser only)*
 * hsv_segment                     : 
        
    *to extrace segemented image*
  * ROI                             : 
        
      *to extract region of interest*
 * extract dehazed image
 * contour_overlay                 : 
        
    *to extract contour identified*
 *  line_overlay                    : 
        
      *to extract line drawn between the contors*

*  original                        : 
        
     *to extract the original image that is passed to laser ranging* 



## 2. image_processing.cpp

### Class ImageProcessing

Class Imageprocessing will have the basic image processing functionalities. This class will be the parent class class for most of the other image processing classes.


**Functions**

  *  CaptureFrame roi_selection(CaptureFrame)                 :
        
    *Crop the region of interset*
  * CaptureFrame hsv_segmentation  (CaptureFrame)              : 
        
     *Segment image according to color set previously (preset for red)*
 *  void set_threshold (CaptureFrame)                : 
        
    *set color threshold according to scene*
 * void set_roi  (int x_percent,int y_percent,int width,int height)                       : 
        
   *Set region of interset*

  ***Public Variables***

 * threshold_high_0                : 
        
    *to set high value of color threshold in 0 hue range*
 * threshold_high_180              : 
        
    *to set high value of color threshold in 180 hue range*
  * threshold_low_0                 : 
        
     *to set low value of color threshold in 0 hue range*
 * threshold_low_180               : 
        
    *to set low value of color threshold in 180 hue range*
 * roi_percentage                  : 
       
    *to set required region of interest percentage*

## 3. algorithm.cpp

### Class Algorithm

Algorithm class will contain all the algorithms needed for image processing. one object of this class will be present in ImageProcessing class.

***Functions***
    
* CaptureFrame CLAHE_dehaze(CaptureFrame)                     : 
        
     *Dehaze using CLAHE algorithm*
 * CaptureFrame hist_equalize (CaptureFrame):                     : 
       
    *Dehaze using normal histogram equalization.*

## 4. view_frame.cpp

### Class ViewFrame

ViewFrame class is used for displaying the images. This class offers multiple modes of displaying including multiple displays(upto 4 frames) and interrupted displays(waits for user input and then continue)

***Functions***

 * void single_view_interrupted   (CaptureFrame)      : 
        
     *Show single window and wait for user input to continue*
 *  void single_view_uninterrupted (CaptureFrame)      : 
        
     *Show single window and continue*
 * void multiple_view_interrupted  (CaptureFrame,CaptureFrame,CaptureFrame,CaptureFrame)     : 
        
     *Show multiple windows and wait for user input to continue*
 * void multiple_view_uninterrupted (CaptureFrame,CaptureFrame,CaptureFrame,CaptureFrame)    : 
        
    *Show multiple windows and continue*
 * CaptureFrame add_overlay_percent (CaptureFrame input, x_percent,y_percent,data)            : 
        
   *Add overlays to the image at a point according to percentage*
 * CaptureFrame add_overlay (CaptureFrame,x_point , y_point, data)                    : 
        
    *Add overlays to the image at the specified point*
        
## 5. capture_frame.cpp

### Class CaptureFrame

CaptureFrame class is used as a substitute for image and video files. It holds one image and one video file and associated functions to load,manipulate,retrieve them.

 **Functions**

  * void capture_image (filename,window_name)                  : 
        
    *load image*
 * void capture_video     (filename,window_name)               : 
    
    *load video*

 * reload_image        (image,window_name)             : 
    
    *rewrite existing image*
 * CaptureFrame retrieve_image                  : 
    
    *extract stored image*
 *  CaptureFrame retrieve_video                  : 
    
     *extract stored video*
 * frame_extraction                : 
    
   *extract frame from video and store it in  image*
    . clear                           : 
    
    *clear all the data in the object
        
## 6. **timer.cpp**    

### Class Timer

Timer class is used for measuring the execution time and maximum fps. It calculates the time between timer initialising and timer ending.


 ***Functions***

 * timer_init ()                     : 
        
     *start the timer*
 * timer_end                ()       : 
        
     *end the timer and calculate execution time and maximum fps*
 * CaptureFrame add_time ()                       :    
       
     *add execution timer as overlay to image*
 * CaptureFrame add_fps   ()                      : 
    
     *add maximum fps as overlay to imgae*

 ***Public Variables***

 *    execution_time                  : 
    
         *to extract the execution time data*

 * fps  :

     *to extract the maximum fps data*
        

