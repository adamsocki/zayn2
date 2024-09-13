void LoadLevel_IMGUI(char* level_path)
{
    std::ifstream file(level_path);
	nlohmann::json sceneData;

	RenderComponent rc = {};

	if (file.is_open())
	{
		file >> sceneData; // read the json file into the sceneData object
		
        for (const auto& obj : sceneData["objects"])
        {
			std::string name = obj["name"];
			std::cout << "Name: " << name << std::endl;

			// Add entity to the scene
			// in order to render, we need the following: 

			// GraphicsPipieline, PipelineLayout, DescriptorSets, RenderComponent, & pushConstantData
			RenderComponent renderComponent = {};

			LoadModel(obj["obj_filePath"], &renderComponent.vertices, &renderComponent.indices);
			for (int i = 0; i < renderComponent.vertices.size(); i++)
			{
				std::cout << "Vertex - x " << " - " <<  i << " : " << renderComponent.vertices[i].pos.x << std::endl;
				std::cout << "Vertex - y " << " - " <<  i << " : " << renderComponent.vertices[i].pos.y << std::endl;
				std::cout << "Vertex - z " << " - " <<  i << " : " << renderComponent.vertices[i].pos.z << std::endl;
			}
			for (int i = 0; i < renderComponent.indices.size(); i++)
			{
				std::cout << "Index - " << i << " : " << renderComponent.indices[i] << std::endl;
			}


			RenderComponent rc = {};
			rc.vertices = renderComponent.vertices;
			rc.indices = renderComponent.indices;
            rc.modelPath = obj["obj_filePath"];
            rc.shader_vert_path = "/Users/socki/dev/zayn/src/renderer/shaders/vert_blank.spv";
            rc.shader_frag_path = "/Users/socki/dev/zayn/src/renderer/shaders/frag_blank.spv";

            CreateVulkanRenderingForEntity(&rc, false);
            PushBack(&Zayn->renderComponents, rc);


		}

		
		
		file.close();
	}
}


void Render_MyIMGUI()
{
// im gui exampes https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();


    ImGui::NewFrame();

    ImGui::ShowDemoWindow();


    // Level Editor Controlls
    ImGui::Begin("Level Editor");

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    if (ImGui::CollapsingHeader("Level Info"))
    {
        
        static int clicked = 0;
        ImGui::SeparatorText("Level Data");
        static char str0[128] = "no level loaded";
        ImGui::LabelText(str0, "Current Level");
        if (ImGui::Button("Save Level Edits"))
        {
            clicked++;
        }
        if (clicked & 1)
        {
            ImGui::SameLine();
            ImGui::Text("Saved!");
        }
        // Load New Level

        char str1[128] = "/Users/socki/dev/zayn/data/levelFiles/level0.txt";
        char str2[128] = "/Users/socki/dev/zayn/data/levelFiles/blender/scene_data.json";
        ImGui::InputText("Level Path", str1, IM_ARRAYSIZE(str1));
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.3f, 0.6f, 0.1f));
        if (ImGui::Button("Load Level"))
        {
            LoadLevel_IMGUI(str2);
        }
        ImGui::PopStyleColor();

    }

    

    ImGui::PopStyleVar();
    ImGui::End();



    // ENTITY EDITOR
    ImGui::Begin("Entity Editor");
    static int clicked_new_entity = 0; 
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 20));
    if (ImGui::Button("Create New Entity"))
    {
        clicked_new_entity++;
    }
    if (clicked_new_entity & 1)
    {
        // create entity
    }

    static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;


    ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
    if (ImGui::CollapsingHeader("Entity List"))
    {
        if (ImGui::BeginTable("table_scrolly", 1, flags, outer_size))
        {

            
        }

        ImGui::EndTable();
    }

    ImGui::PopStyleVar();

    
    ImGui::End();
    

    ImGui::Render();

};