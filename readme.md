
LASER RANGING
-----------------    
-----------------   
                                    
<div style="text-align: right"> Planys Technologies </div>

## USAGE

>make

>./LaserRanging <path to video or image>


## INCLUDED FILES

   

1. laser_ranging.cpp   
            
    ***Functions***

    * contour_distance                :
    
        *Identify contours and distance between them in a segmented image*              
    * contour_distance_single_laser   : 
    
        *Identify contours and distance between them in a segmented image for single laser ranging enabled*
    * get_laser_distance              : 
    
        *Distance from two laser dots*
    * get_left_laser_distance         : 
    
        *Distance according to left laser dot*      
    * get_right_laser_distance        : 
    
        *Distance according to right laser dot*  
    *    laser_ranging                   : 
    
            *Single function to call all associated functions for laser ranging*   
    * laser_ranging_single_laser      : 
    
        *Single function to call all associated functions for laser ranging with single laser ranging enabled*
    * show_overlay                    : 
    
        *Show overlays associated with laser ranging*   
    * show_overlay_single_laser       : 
    
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

    *  roi_selection                   :
        
        *Crop the region of interset*
    *  hsv_segmentation                : 
        
        *Segment image according to color set previously (preset for red)*
    *  set_threshold                   : 
        
        *set color threshold according to scene*
    * set_roi                         : 
        
        *Set region of interset percentage*

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
    * CLAHE_dehaze                     : 
        
        *Dehaze using CLAHE algorithm*
    * hist_equalize:                     : 
       
        *Dehaze using normal histogram equalization.*
4. view_frame.cpp

    * **single_view_interrupted**         : 
        
        *Show single window and wait for user input to continue*
    * **single_view_uninterrupted**       : 
        
        *Show single window and continue*
    * multiple_view_interrupted       : 
        
        *Show multiple windows and wait for user input to continue*
    * multiple_view_uninterrupted     : 
        
        *Show multiple windows and continue*
    * add_overlay_percent             : 
        
        *Add overlays to the image at a point according to percentage*
    * add_overlay                     : 
        
         *Add overlays to the image at the specified point*
        
5. capture_frame.cpp

    **Functions**
   * capture_image                   : 
        
        *load image*
    * capture_video                   : 
    
        *load video*

    * reload_image                    : 
    
        *rewrite existing image*
    * retrieve_image                  : 
    
        *extract stored image*
    *  retrieve_video                  : 
    
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
    * add_time                        :    
       
        *add execution timer as overlay to image*
    * add_fps                         : 
    
        *add maximum fps as overlay to imgae*

    ***Public Variables***
    *    execution_time                  : 
    
            *to extract the execution time data*

    * fps  :

        *to extract the maximum fps data*
        

