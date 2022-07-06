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
	
	
	[Attribute(defvalue: "1.0", uiwidget : UIWidgets.EditBox, desc: "Alpha Test", category: "Material Settings")];
	float alphaTest;
	
	[Attribute(defvalue: "1.0", uiwidget : UIWidgets.EditBox, desc: "Alpha Mul", category: "Material Settings")];
	float alphaMul;
	
	[Attribute(defvalue: "AlphaBlend", uiwidget : UIWidgets.EditBox, desc: "Alpha Blend Mode", category: "Material Settings")];
	string blendMode;
	
	//0.136 0 0 1
	[Attribute(defvalue: "0.76 0.004 0.004 1", uiwidget : UIWidgets.Auto, desc: "Color", category: "Material Settings")];
	string colorMaterial;		//color pciker doesnt work dunno why
		
	[Attribute(defvalue: "0", uiwidget : UIWidgets.EditBox, desc: "Normal Power", category: "Material Settings")];
	float normalPower;
	
	[Attribute(defvalue: "0", uiwidget : UIWidgets.EditBox, desc: "Specular Multiplier", category: "Material Settings")];
	float specularMul;
	
	[Attribute(defvalue: "0", uiwidget : UIWidgets.EditBox, desc: "Roughness Scale", category: "Material Settings")];
	float roughnessScale;
	
	[Attribute(defvalue: "0", uiwidget : UIWidgets.EditBox, desc: "Metalness Scale", category: "Material Settings")];
	float metalnessScale;
	
	
	[Attribute(defvalue: "0", uiwidget : UIWidgets.EditBox, desc: "AO Scale", category: "Material Settings")];
	float aoScale;
	
	[Attribute(defvalue: "0", uiwidget : UIWidgets.EditBox, desc: "Dielectric Reflectance", category: "Material Settings")];
	float dielectricReflectance;
	
	
	[Attribute(defvalue: "0", uiwidget : UIWidgets.CheckBox, desc: "GBuffer Normal", category: "GBuffer Override")];
	bool gBufferNormal;
	
	[Attribute(defvalue: "0", uiwidget : UIWidgets.EditBox, desc: "Normal Combine Power", category: "GBuffer Override")];
	float normalCombinePower;
	
	
	
		
	[Attribute(defvalue: "1", uiwidget : UIWidgets.EditBox, desc: "Frame to start with", category: "Temp Fixes")];
	int offsetFrame;
	
	
	[Attribute(defvalue: "0", uiwidget : UIWidgets.CheckBox, desc: "Skip materials generation", category: "Temp Fixes")];
	bool skipMatGen;
	
	[Attribute(defvalue: "0", uiwidget : UIWidgets.CheckBox, desc: "Import Texture", category: "Temp Fixes")];
	bool importTexture;
	
	[Attribute(defvalue: "0", uiwidget : UIWidgets.CheckBox, desc: "000 index format", category: "Temp Fixes")];
	bool compatIndex;
	
	
	
	ResourceManager resourceManager; 
	

	[ButtonAttribute("Run")]
	
	void RunButton()	 
	
	{
		
	
		
		string arrayResourceNameString = "array<ResourceName> materials = {"; // i hate this ide so much.
			

		array<string> bcrArray = {};
		array<string> nmoArray = {};
		array<string> opArray = {};
			

		System.FindFiles(bcrArray.Insert, folderBCR.GetPath(), ".edds");
			
		System.FindFiles(nmoArray.Insert, folderNMO.GetPath(), ".edds");
		System.FindFiles(opArray.Insert, folderOpacity.GetPath(), ".edds");
			
			
		bcrArray.Sort();
		nmoArray.Sort();
		opArray.Sort();
			
		
			
			
			
		//really need a value
		string bcrBaseString = bcrArray[0];
		bcrBaseString.Replace("000","%1");		
		bcrBaseString.Replace("001","%1");		

			
		if (!compatIndex)
		{
			bcrBaseString.Replace("0", "%1");
			bcrBaseString.Replace("_1", "_%1");		
			bcrBaseString.Replace("_01","_%1");		
		}


		string nmoBaseString = nmoArray[0];

		nmoBaseString.Replace("001","%1");		
		nmoBaseString.Replace("000","%1");		

		if (!compatIndex)
		{
			nmoBaseString.Replace("0", "%1");
			nmoBaseString.Replace("_1", "_%1");		
			nmoBaseString.Replace("_01", "_%1");	
		}


		string opacityBaseString = opArray[0];
		opacityBaseString.Replace("000","%1");		
		opacityBaseString.Replace("001","%1");	

		
		if (!compatIndex)
		{
			opacityBaseString.Replace("0", "%1");
			opacityBaseString.Replace("_1", "_%1");		
			opacityBaseString.Replace("_01", "_%1");	
		}


			
		int numFrames =  bcrArray.Count();
			
		//Print("Selected "+ numOfFrames.ToString() + "frames");
			
			
		for(int i = offsetFrame; i < numFrames + offsetFrame; i++)
		{
			string correctIndex = i.ToString();

				
			if (compatIndex)
			{
				
				if (i < 10)
					correctIndex = "00" + i.ToString();
				else if (i < 100)
					correctIndex =  "0" + i.ToString();
				else
					correctIndex = i.ToString();
			}
				

			//
			//Print(correctIndex);
			string currentFilePath;
				
			if (importTexture)
			{
				currentFilePath = saveFolder.GetPath() + "/" + correctIndex + ".edds";

			}
			else 
			{
				currentFilePath = saveFolder.GetPath() + "/" + correctIndex + ".emat";	
			}

			if (!skipMatGen)	
			{
				ResourceName bcrTemp = string.Format(bcrBaseString, correctIndex);
				ResourceName opacityTemp = string.Format(opacityBaseString, correctIndex);
				ResourceName nmoTemp = string.Format(nmoBaseString, correctIndex);
				
	
				string correctString = "MatPBRDecal {\n";
	
						
				correctString += string.Format("BCRMap \"%1\"\n", bcrTemp);	
				correctString += string.Format("OpacityMap \"%1\"\n", opacityTemp);	
				correctString += string.Format("NMOMap \"%1\"\n", nmoTemp);	
				correctString += "Color " + colorMaterial + "\n";
				correctString += "SpecularMul " + specularMul.ToString() + "\n";
				correctString += "RoughnessScale " + roughnessScale.ToString() + "\n";
				correctString += "MetalnessScale " + metalnessScale.ToString() + "\n";
				correctString += string.Format("DielectricReflectance %1 \n", dielectricReflectance);
				correctString += string.Format("AOScale %1 \n", aoScale.ToString());
				correctString += "NormalPower " + normalPower.ToString() + "\n"; 
				correctString += "BlendMode " + blendMode + "\n";
				correctString += "AlphaTest " + alphaTest.ToString() + "\n";
				correctString += string.Format("AlphaMul %1\n", alphaMul);
				correctString += string.Format("GBufferNormal %1\n", gBufferNormal);
				correctString += string.Format("NormalCombinePower %1\n", normalCombinePower);
						

				correctString += "}";
	
				FileHandle currentMatFile = FileIO.OpenFile(currentFilePath, FileMode.WRITE);
				if (currentMatFile != 0)
				{
					Print("Saving mat " + i.ToString());
					currentMatFile.FPrint(correctString);
					currentMatFile.CloseFile();

	
				}
					
				
			}					
			
			
			
			ResourceName tempMat = Workbench.GetResourceName(currentFilePath);
			string absPath;
			Workbench.GetAbsolutePath(currentFilePath, absPath, false);
			MetaFile meta = resourceManager.RegisterResourceFile(absPath);
				
				
				
			if (i + 1 >= numFrames + offsetFrame)
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
		resourceManager = Workbench.GetModule(ResourceManager);
		if (!resourceManager) 
			return;
		

		Workbench.ScriptDialog("Batch Material Importer", "Import multiple materials rapidly", this);
		

	}


	// Code which is executed when settings are accesed
	override void Configure()
	{
		//Workbench.ScriptDialog("Plugin script dialog title", "Description of the plugin\nThis description can use multiple lines.\nPress export to copy plugin settings to clipboard.\nPress import to grab data from clipboard.", this);
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




