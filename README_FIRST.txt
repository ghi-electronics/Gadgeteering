During BETA development & Releases please ASSUME that 
    * Naming conventions might change
	* the actual layout tree and names of the repository might change
	
For instance, we are planning to start on 11/6/2013 to move all 
symbol names to lower case names with an underscore between words.
Manifest Constants will be all UPPER case.

On 11/6/2013 we will start re-organizing the repository in a way that
looks more like

      root
	  core modules tests examples contributed tools
	  
we will be sharing scripts that will, as needed, re-organize the 
repository on your installation to make development easier/conforming
to the target boards and/or development platform. A first example
of this will be a script (batch or sh) to  pull module source 
(headers & code) into a directory structure for FEZ Medusa products
to match assumptions made by the Arduino IDE.