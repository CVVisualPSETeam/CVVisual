#Introduction to filter function widgets
##The class, functions and types  
If you want to enable the user to provide input to a filter you can inherit the virtual class FilterFunctionWidget<In,Out>.
It provides an interface for a filter accepting In images as an input and Out images as output.
The input images are passed using

	InputArray  = std::array<util::Reference<const cv::Mat>,In>
and the output is provided with an output parameter of the type:

	OutputArray = std::array<util::Reference<cv::Mat>,Out>

You should override following functions:

	virtual void applyFilter(InputArray in,OutputArray out) const;
	
	virtual std::pair<bool, QString> checkInput(InputArray in) const;

appyFilter has to apply your filter and checkInput should check wheather the filter can be applyed (the first member of the returned pair).
In case the filter can not be applyed the second member of the returned pair should contain a message for the user.

If user input changes the setting of the filter the function _emitSignal()_ of the member signFilterSettingsChanged\_ should be called.

For a detailed example look at CVVisual/src/qtutil/filter/grayfilterwidget.{hpp, cpp}
