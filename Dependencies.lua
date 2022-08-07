-- Moon Engine Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["Box2D"] = "%{wks.location}/Engine/Moon-Engine/vendor/Box2D/include"
IncludeDir["entt"] = "%{wks.location}/Engine/Moon-Engine/vendor/entt/include"
IncludeDir["FastNoise"] = "%{wks.location}/Engine/Moon-Engine/vendor/FastNoise"
IncludeDir["Glad"] = "%{wks.location}/Engine/Moon-Engine/vendor/Glad/include"
IncludeDir["GLFW"] = "%{wks.location}/Engine/Moon-Engine/vendor/GLFW/include"
IncludeDir["glm"] = "%{wks.location}/Engine/Moon-Engine/vendor/glm"
IncludeDir["ImGui"] = "%{wks.location}/Engine/Moon-Engine/vendor/ImGui"
IncludeDir["ImGuizmo"] = "%{wks.location}/Engine/Moon-Engine/vendor/ImGuizmo"
IncludeDir["msdf_atlas_gen"] = "%{wks.location}/Engine/Moon-Engine/vendor/msdf-atlas-gen/msdf-atlas-gen"
IncludeDir["msdfgen"] = "%{wks.location}/Engine/Moon-Engine/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["shaderc"] = "%{wks.location}/Engine/Moon-Engine/vendor/shaderc/include"
IncludeDir["spdlog"] = "%{wks.location}/Engine/Moon-Engine/vendor/spdlog/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Engine/Moon-Engine/vendor/SPIRV-Cross"
IncludeDir["stb_image"] = "%{wks.location}/Engine/Moon-Engine/vendor/stb_image"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["yaml"] = "%{wks.location}/Engine/Moon-Engine/vendor/yaml/include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
