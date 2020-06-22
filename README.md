# Application for manual segmentation

**PROJECT:** 

In this project, an application was developed that performs manual segmentation in RGB images. It also allows you to edit the segmentation in progress and, finally, saves the segmentation as a binary image. The application was implemented in C++, using Qt as a graphical UI.

**FEATURES:** 

* Manual segmentation in an RGB image;
* Edit the current segmentation, from deleting the last selected point (and line), until deleting all the segmentation performed;
* Define a new segmentation, different from the segmentation previously created;
* View the three image channels (RGB);
* Save the segmentation performed and view the final result.

**FILES:** 
* [aplicacao.pro](https://github.com/MiguelCastro3/Application-for-manual-segmentation/blob/master/aplicacao.pro) - application project file.
* [aplicacao.h](https://github.com/MiguelCastro3/Application-for-manual-segmentation/blob/master/aplicacao.h) and [aplicacao.cpp](https://github.com/MiguelCastro3/Application-for-manual-segmentation/blob/master/aplicacao.cpp) - class aplicacao with the code for the back-end of the developed application and connection with the graphics_scene class. 
* [aplicacao.ui](https://github.com/MiguelCastro3/Application-for-manual-segmentation/blob/master/aplicacao.ui) - interface implemented for the front-end of the final application.
* [graphics_scene.h](https://github.com/MiguelCastro3/Application-for-manual-segmentation/blob/master/graphics_scene.h) and [graphics_scene.cpp](https://github.com/MiguelCastro3/Application-for-manual-segmentation/blob/master/graphics_scene.cpp) - which defines the scene where the items will be viewed and allows the manipulation of it.


**RESULTS:** 

![1](https://user-images.githubusercontent.com/66881028/84962699-5f492680-b0ff-11ea-94bb-f41b7b05945b.png)

![2](https://user-images.githubusercontent.com/66881028/84962703-6112ea00-b0ff-11ea-9829-bdbe8efc4b3a.png)

![3](https://user-images.githubusercontent.com/66881028/84962704-61ab8080-b0ff-11ea-9614-5cc0600b774a.png)
