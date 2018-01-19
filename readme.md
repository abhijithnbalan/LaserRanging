
LASER RANGING
-----------------    
-----------------   
                                    
<div style="text-align: right"> Planys Technologies </div>

## USAGE
```bash
make

./LaserRanging <path to video or image>
```

## INCLUDED FILES

   

1. laser_ranging.cpp   
            
    ***Functions***

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

2. image_processing.cpp

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

 3. algorithm.cpp


    ***Functions***
    * CaptureFrame CLAHE_dehaze(CaptureFrame)                     : 
        
        *Dehaze using CLAHE algorithm*
    * CaptureFrame hist_equalize (CaptureFrame):                     : 
       
        *Dehaze using normal histogram equalization.*
4. view_frame.cpp

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
        
5. capture_frame.cpp

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
        
6. **timer.cpp**    

    ***Functions***
    * timer_init                      : 
        
        *start the timer*
    * timer_end                       : 
        
        *end the timer and calculate execution time and maximum fps*
    * CaptureFrame add_time                        :    
       
        *add execution timer as overlay to image*
    * CaptureFrame add_fps                         : 
    
        *add maximum fps as overlay to imgae*

    ***Public Variables***
    *    execution_time                  : 
    
            *to extract the execution time data*

    * fps  :

        *to extract the maximum fps data*
        

