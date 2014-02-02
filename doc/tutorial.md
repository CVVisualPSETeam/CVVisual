#Introduction to using CVVisual
##Opening the debug window  
Open the debug window by putting one of the functions from the CVVisual API into your code.    
In this example, we want to debug a call to 'dilate' (line.2) so we use debug_dilate.  
###Example: Code  
src, dest and the structuring element elem are of type cv::Mat (see the OpenCV doc on [dilate()](http://docs.opencv.org/modules/imgproc/doc/filtering.html#dilate); description is a char array.  

	2 cv::dilate(src, dest, elem);  
	3 cvv::debugDilate(src, dest, CVVISUAL_LOCATION, description);    
When executing the code, the debug_dilate function will open the window and halt the execution.  
##The Overview Tab  
You are now in the overview tab. Each time you call one of the CVVisual functions, a *Call* is added to the table.  
You can see the images you passed to the funtion as well as metadata and additional information.  
The text field allows you to sort or group the Calls by different criteria; see the STFL documentation on how to use it.  
Now double-click on the Call.
##Debugging a filter operation  
This is a *CallTab*. In the center, there are the images from the call.
In the *View* drop-down menu you find different *Views* of the Call, that is, different visualizations of it. The accordion menu on the left offers additional options, depending on the View and the type of the Call.  
As our dilate seems to have produced acceptable results, we want to continue through the code.  
So, we push the `Resume program execution` button in the upper left.  
The window will comes up again the next time one of the CVVisual functions is called.
Then, we see two Calls in the Overview table, the one from before and the new one.  
If this was the last call to a function of CVVisual, instead of `Resume program execution` a button labeled `Exit program` will appear, which does exactly what it says.  