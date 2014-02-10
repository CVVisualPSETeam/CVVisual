#Introduction to using CVVisual
##Enabling debug mode  
Define the CVV\_DEBUG_MODE macro somewhere in the translation unit.
##Opening the debug window  
Open the debug window by putting one of the functions from the [CVVisual API](/api) into your code.    
In this example, we want to debug a call to 'dilate' (line.1) so we use debug_dilate.  
###Example: Code  
src, dest and the structuring element elem are of type cv::Mat (see the OpenCV doc on [dilate()](http://docs.opencv.org/modules/imgproc/doc/filtering.html#dilate)); CVVISUAL_LOCATION and filename are char arrays.  

	0 #include <filter.hpp>
	1 cv::dilate(src, dest, elem);  
	2 cvv::debugFilter(src, dest, CVVISUAL_LOCATION, filename);    
When executing the code, the debug_dilate function will open the window and halt the execution.  
##The Overview Tab 
![](images_tut/dilate_overview.png)
You are now in the overview tab. Each time you call one of the CVVisual functions, a *Call* is added to the table.  
You can see the images you passed to the funtion as well as metadata and additional information.  
The text field allows you to sort or group the Calls by different criteria; see the [filter query language documentation](filterquery-ref.html) on how to use it.  
Now double-click on the Call or select `Open in 'CVVisual|main window'` from the context menu.  
(You can also choose to remove the Call or open it in a new window there)  
##Debugging a filter operation  
![](images_tut/dilate_calltab_defaultfview.png)
A *CallTab* opens. In the center, there are the images from the call.
In the `View` drop-down menu you find different *Views* of the Call, that is, different visualizations of it. The accordion menu on the left offers information on the images and additional options depending on the View and the type of the Call.
Important here might be that `ImageInformation` offers the possibility to zoom (you can also use the mouse wheel); if you zoom in more than 60%, the image pixels will be overlaid with the channel values, in a 3-channel image usually in order (top-down) BGR.  
As our dilate seems to have produced acceptable results, we want to continue through the code.  
So, we push the `Resume program execution` button in the upper left.  
The window will comes up again the next time one of the CVVisual functions is called.
Then, we see two Calls in the Overview table, the one from before and the new one.  
  
Put `finalShow()` after the last CVVisual function.
If the program reaches it, instead of `Resume program execution` a button labeled `Exit program` will appear, which does exactly what it says.  
  
([Source](http://commons.wikimedia.org/wiki/File:PNG-Gradient.png) of the image used for demonstration) 
