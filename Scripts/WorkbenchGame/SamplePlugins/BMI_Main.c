
[WorkbenchPluginAttribute(name: "Batch Material Creator", category: "Pao's Plugins", shortcut: "Ctrl+T", wbModules: {"ResourceManager"}, awesomeFontCode: 0xf0c5)]
class BatchMaterialCreator: ResourceManagerPlugin
{
	[Attribute("SampleFolderBCR", UIWidgets.Auto, "BCR Folder")]
	string folderBCR;
	
	[Attribute("SampleFolderNMO", UIWidgets.Auto, "NMO Folder")]
	string folderNMO;
	
	
	[Attribute("SampleFolderOpacity", UIWidgets.Auto, "Opacity Folder")]
	string folderOpacity;
	
	[Attribute("SampleSaveFolder", UIWidgets.Auto, "Save Folder")]
	string saveFolder;
	
	
	
	[Attribute("TEMP", UIWidgets.Auto, "Name start with")]
	string startName;
	
	
	[Attribute("1", UIWidgets.Auto, "Number of frames")]
	int numOfFrames;
	
	[Attribute("1", UIWidgets.Auto, "Temp to fix screwups")]
	int offsetFrame;
	
	
	[Attribute("1.0", UIWidgets.Auto, "Temp to fix screwups")]
	float alphaTest;
	
	
	
	
	// Plugins settings - those can be changed in Plugins -> Settings section 
	[Attribute("0", UIWidgets.CheckBox, "Check this option to print output to clipboard.")]
	bool m_CopyToClipboard;
	
	[Attribute("0", UIWidgets.CheckBox, "Check this option to print output array to the console log.")]
	bool m_PrintToConsole;
	
	
	[ButtonAttribute("Run")]
	void RunButton()	 
	{
		
		
		//should store EVERYTHING in 3 arrays 
		//array<ResourceName> bcrMaps = new array<ResourceName>();
		//array<ResourceName> nmoMaps = new array<ResourceName>();
		//array<ResourceName> opacityMaps = new array<ResourceName>();
		
		
		
		// we could get a default mat to use as a base instead of this crap
		
		
		map<string, string> matParams = new map<string, string>();
		
		
		
		
		// todo need to support more than 100 tiles.
		
	
		Print("Selected "+ numOfFrames.ToString() + "frames");
		for(int i = offsetFrame; i < numOfFrames; i++)
		{
			
			
			
			string correctIndex;
			
			
			if (i < 10)
				correctIndex = "00" + i.ToString();
			else if (i < 100)
				correctIndex = "0" + i.ToString();
			
			Print(correctIndex);
			
			
			
			//FileHandle handleSingleFile = FileIO.OpenFile(startName + correctIndex + ".edds", FileMode.READ);
			
			ResourceName bcrTemp = Workbench.GetResourceName(folderBCR + "/" + startName + correctIndex + ".edds");
			ResourceName opacityTemp = Workbench.GetResourceName(folderOpacity + "/" + startName + correctIndex + ".edds");
			ResourceName nmoTemp = Workbench.GetResourceName(folderNMO + "/" + startName + correctIndex + ".edds");
			
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
				 
			

			
			
			string currentFilePath = saveFolder + "/" + i.ToString() + ".emat";
			FileHandle currentMatFile = FileIO.OpenFile(currentFilePath, FileMode.WRITE);
			if (currentMatFile != 0)
			{
				Print("Saving mat " + i.ToString());
				currentMatFile.FPrint(correctString);
				currentMatFile.CloseFile()
			}

			
			
		
		}
		
		

	
	}
	
	
	// This code is executed when plugin is executed either by clicking on it in Plugins list or when shortcut is used
	override void Run()
	{
		// Grab reference to ResourceManager 
		ResourceManager resourceManager = Workbench.GetModule(ResourceManager);
		if (!resourceManager) 
			return;
		

		
		Workbench.ScriptDialog("BMI", "", this);


	}


	// Code which is executed when settings are accesed
	override void Configure()
	{
		Workbench.ScriptDialog("Plugin script dialog title", "Description of the plugin\nThis description can use multiple lines.\nPress export to copy plugin settings to clipboard.\nPress import to grab data from clipboard.", this);
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
		if(m_PrintToConsole) 
		{
			// Print directly to the Log Console absolute path & file name of newely registerd resource
			Print(absFileName);
		}
		if(m_CopyToClipboard)
		{
			// Copy file name to clipboard
			System.ExportToClipboard(absFileName);
		}
	}
	
	
	// ButtonAttributes 
	[ButtonAttribute("Cancel")]
	void CancelButton()	 
	{
		return;
	
	}
}



[WorkbenchPluginAttribute(name: "Multiple Materials Importer", category: "Pao's Plugins",  wbModules: {"WorldEditor"}, awesomeFontCode: 0xf0c5)]
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
	protected int offset;
	
	

	
	[ButtonAttribute("Setup Object")]
	void SetupObject()
	{
		ECM_AnimatedDecalsManager entity = ECM_AnimatedDecalsManager.Cast(api.CreateEntity("ECM_AnimatedDecalsManager", "", api.GetCurrentEntityLayerId(), null, vector.Zero, vector.Zero));
		entity.m_Frames = new array<ResourceName>();

		for(int i = offset; i < numberOfFrames; i++)
		{
			Print(i);
			ResourceName tempMat = Workbench.GetResourceName(materialsFolder + "/" + i.ToString() + ".emat");
			entity.m_Frames.Insert(tempMat);

		}

		// save config
		string currentFilePath = saveFolder + "/" + configFileName + ".conf";
		FileHandle confFile = FileIO.OpenFile(currentFilePath, FileMode.WRITE);
		if (confFile != 0)
		{
			confFile.FPrint("");		//Just to let it save I guess
			confFile.CloseFile();
		}

		Resource holder = BaseContainerTools.CreateContainerFromInstance(entity);
		if (holder)
			BaseContainerTools.SaveContainer(holder.GetResource().ToBaseContainer(), currentFilePath);
		
	

	}
	
	
	override void Run()
	{
		worldEditor = Workbench.GetModule(WorldEditor);
		api = worldEditor.GetApi();

			
		Workbench.ScriptDialog("Configure settings", "", this);

		
	}
	
	
	
	
	
	

}









