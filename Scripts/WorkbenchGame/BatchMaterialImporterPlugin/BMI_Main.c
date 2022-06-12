
[WorkbenchPluginAttribute(name: "Batch Material Creator", category: "Pao's Plugins", wbModules: {"ResourceManager"}, awesomeFontCode: 0xf0c5)]
class BatchMaterialCreator: ResourceManagerPlugin
{

	[Attribute(defvalue: "SampleFolderBCR", uiwidget : UIWidgets.ResourceNamePicker, desc: "BCR Folder", params: "folders", category: "Directories")];
	ResourceName folderBCR;
	
	[Attribute(defvalue: "SampleFolderNMO", uiwidget : UIWidgets.ResourceNamePicker, desc: "NMO Folder", params: "folders", category: "Directories")];
	ResourceName folderNMO;
	
	[Attribute(defvalue: "SampleFolderOpacity", uiwidget : UIWidgets.ResourceNamePicker, desc: "Opacity Maps Folder", params: "folders", category: "Directories")];
	ResourceName folderOpacity;
	
	[Attribute(defvalue: "SampleSaveLocation", uiwidget : UIWidgets.ResourceNamePicker, desc: "Output location", params: "folders", category: "Directories")];
	ResourceName saveFolder;
	
	
	[Attribute(defvalue: "SampleSaveLocation", uiwidget : UIWidgets.Auto, desc: "Frame name starts with", category: "Frames")];
	string startName;
	
	[Attribute(defvalue: "1", uiwidget : UIWidgets.EditBox, desc: "Number of frames", category: "Frames")];
	int numOfFrames;
	
	
	[Attribute(defvalue: "1.0", uiwidget : UIWidgets.EditBox, desc: "Alpha for Materials", category: "Material Settings")];
	float alphaTest;
		

	[Attribute(defvalue: "1", uiwidget : UIWidgets.EditBox, desc: "Frame to start with", category: "Temp Fixes")];
	int offsetFrame;
	

	[ButtonAttribute("Run")]
	void RunButton()	 
	{
		map<string, string> matParams = new map<string, string>();
		string arrayResourceNameString = "array<ResourceName> materials = {";

	
		Print("Selected "+ numOfFrames.ToString() + "frames");
		for(int i = offsetFrame; i < numOfFrames; i++)
		{
			string correctIndex;
			
			if (i < 10)
				correctIndex = "00" + i.ToString();
			else if (i < 100)
				correctIndex = "0" + i.ToString();
			
			Print(correctIndex);
						
			ResourceName bcrTemp = Workbench.GetResourceName(folderBCR.GetPath() + "/" + startName + correctIndex + ".edds");
			ResourceName opacityTemp = Workbench.GetResourceName(folderOpacity.GetPath() + "/" + startName + correctIndex + ".edds");
			ResourceName nmoTemp = Workbench.GetResourceName(folderNMO.GetPath() + "/" + startName + correctIndex + ".edds");
			
			matParams.Set("BCRMap", bcrTemp);
			matParams.Set("OpacityMap", opacityTemp);
			matParams.Set("NMOMap", nmoTemp);
		
			Material newMat = Material.Create(i.ToString(), "MatPBRDecal", matParams);

			string correctString = "MatPBRDecal {\n";
			correctString += "BCRMap \"" + bcrTemp + "\"\n";
			correctString += "OpacityMap \"" + opacityTemp + "\"\n";
			correctString += "NMOMap \"" + nmoTemp + "\"\n";
			correctString += "AlphaTest " + alphaTest.ToString() + "\n";
			correctString += "}";

			string currentFilePath = saveFolder.GetPath() + "/" + i.ToString() + ".emat";
			FileHandle currentMatFile = FileIO.OpenFile(currentFilePath, FileMode.WRITE);
			if (currentMatFile != 0)
			{
				Print("Saving mat " + i.ToString());
				currentMatFile.FPrint(correctString);
				currentMatFile.CloseFile()
			}
			
			
			
			ResourceName tempMat = Workbench.GetResourceName(currentFilePath);
			
			if (i + 1 >= numOfFrames)
				arrayResourceNameString += "'" + tempMat + "'};";
			else
				arrayResourceNameString += "'" + tempMat + "',";		//can't escape ", so yeah, replace it later with notepad++ or stuff like that

		}

		// Saves a txt with the string in it. Currently kinda broken but yyyyyyyyyeah
		string stringArrayFinalPath = saveFolder.GetPath() + "/materials.txt";
		FileHandle currentMatFile = FileIO.OpenFile(stringArrayFinalPath, FileMode.WRITE);
		if (currentMatFile != 0)
		{
			Print("Saving text with everything!");
			currentMatFile.FPrint(arrayResourceNameString);
			currentMatFile.CloseFile()
		}
	}
	
	
	override void Run()
	{
		// Grab reference to ResourceManager 
		ResourceManager resourceManager = Workbench.GetModule(ResourceManager);
		if (!resourceManager) 
			return;
		

		Workbench.ScriptDialog("Batch Material Importer", "Import multiple materials rapidly", this);
		

	}


	// Code which is executed when settings are accesed
	override void Configure()
	{
		//Workbench.ScriptDialog("Plugin script dialog title", "Description of the plugin\nThis description can use multiple lines.\nPress export to copy plugin settings to clipboard.\nPress import to grab data from clipboard.", this);
	}
	
	override void RunCommandline()
	{
	    ResourceManager resourceManager = Workbench.GetModule(ResourceManager);
		
		
	 
		// Default values
		string param = "$ArmaReforger:";
		string autoclose = "0";
		
		// First parameter called myParameter
		resourceManager.GetCmdLine("-myParameter", param);
		resourceManager.GetCmdLine("-autoclose", autoclose);
		
		// Print parameters in console
		PrintFormat("CLI parameters -myParameter= %1 -autoClose=%2",param,autoclose);
		
		// Find any .et (prefab) files in selected location
		array<string> files = {};
		System.FindFiles(files.Insert, param, ".et");
		int numberOfFiles = files.Count();
		
		// Print number of all files to Log Console
		Print(numberOfFiles);
	 
		// Export to clipboard result of the search
		System.ExportToClipboard("Number of all .et files in " + param + " = " + numberOfFiles.ToString());
		
		// Close workbench if autoclose parameter is set to 1
	    if (autoclose == "1")
	        Workbench.Exit(0);
	}
	
	// This method is executed every time some new resource is registered
	override void OnRegisterResource(string absFileName, BaseContainer metaFile)
	{
		
		Print(absFileName);
		

	}
	
	
	// ButtonAttributes 
	[ButtonAttribute("Cancel")]
	void CancelButton()	 
	{
		return;
	
	}
}

/*
[WorkbenchPluginAttribute(name: "Multiple Materials Importer", category: "Pao's Plugins", shortcut: "Ctrl+T", wbModules: {"ResourceManager"}, awesomeFontCode: 0xf0c5)]
class MultipleMaterialsImporter: WorldEditorPlugin
{
	
	// Get World Editor module
	WorldEditor worldEditor;
	// Get World Editor API
	WorldEditorAPI api;
	
	
	[Attribute("Temp", UIWidgets.Auto, "Folder where to find materials")]
	protected string materialsFolder;
	
	[Attribute("SampleSaveFolder", UIWidgets.Auto, "Save Folder")]
	string saveFolder;
	
	[Attribute("decal_test", UIWidgets.Auto, "Folder where to find materials")]
	protected string configFileName;
	
	
	[Attribute("0", UIWidgets.Auto, "Folder where to find materials")]
	protected int numberOfFrames;
		
	[Attribute("0", UIWidgets.Auto, "Folder where to find materials")]
	protected int offsetFrame;
	
	

	
	[ButtonAttribute("Setup Object")]
	void SetupObject()
	{

		string correctString = "array<ResourceName> materials = {";
		for(int i = offsetFrame; i < numberOfFrames; i++)
		{
			

			
			string correctIndex;
			
			
			if (i < 10)
				correctIndex = "00" + i.ToString();
			else if (i < 100)
				correctIndex = "0" + i.ToString();
			
			Print(correctIndex);
			
			ResourceName tempMat = Workbench.GetResourceName(materialsFolder + "/" + i.ToString() + ".emat");
				
				
			if (i + 1 > numberOfFrames)
				correctString += "};";
			else
				correctString += "'" + tempMat + "',";
			
			
			//fucking hell /" doesnt work, so yeah 
			
			
			


		
		}
		
		
		
		string currentFilePath = saveFolder + "/materials.txt";
		FileHandle currentMatFile = FileIO.OpenFile(currentFilePath, FileMode.WRITE);
		if (currentMatFile != 0)
		{
			Print("Saving text with everything!");
			currentMatFile.FPrint(correctString);
			currentMatFile.CloseFile()
		}
		
		
		
		
	

	}
	
	
	override void Run()
	{
		worldEditor = Workbench.GetModule(WorldEditor);
		api = worldEditor.GetApi();

			
		Workbench.ScriptDialog("Configure settings", "", this);

		
	}
	
	
	
	
	
	

}




*/




