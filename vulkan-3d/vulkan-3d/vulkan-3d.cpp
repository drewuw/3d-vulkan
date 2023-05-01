#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <optional>
#include <vector>
#include <string>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

const uint32_t WIDTH = 3200;
const uint32_t HEIGHT = 1800;

class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow* window;
	VkInstance instance;
	void initWindow() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // GLFW_NO_API tells GLFW to not create an OpenGL context but rather to only create a window
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}
	void createInstance() { //this fucntion creates an instance of the Vulkan library
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // VK_STRUCTURE_TYPE_APPLICATION_INFO is a constant that tells Vulkan which structure you are using, which allows the implementation to read the data accordingly
		appInfo.pApplicationName = "Hello Triangle"; //the "p" is a naming convention that indicates a pointer to a null-terminated string
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine"; //an example would be Unreal, Unity or Godot
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		uint32_t glfwExtensionCount = 0; //uint32_t is an unsigned 32 bit integer
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0;

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) { // if instance creation fails
			throw std::runtime_error("failed to create instance!");
		}
	}

	struct QueueFamilyIndices { // this struct is used to find the queue families that are supported by the device
		std::optional<uint32_t> graphicsFamily;

		bool isComplete() {
			return graphicsFamily.has_value();
		}
	};

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr); //this function gets the number of queue families and stores it in queueFamilyCount
		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data()); //passes device, queueFamilyCount and queueFamilies.data() as pointers to the function
		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) { //& bitwise AND operator to check if the queue family supports graphics. if it does then it will be non-zero
				indices.graphicsFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices; //return the indices/position of the queue family that supports graphics
	}

	bool isDeviceSuitable(VkPhysicalDevice device) {
		QueueFamilyIndices indices = findQueueFamilies(device);
		return indices.isComplete(); //checks if the quefamilies have all been searched and if the graphics family has been found
	}


	void initVulkan() {
		createInstance();
	}
	void mainLoop() {
		while (!glfwWindowShouldClose(window)) { // while window is not closed
			glfwPollEvents(); // this function checks if any events are triggered (like keyboard input or mouse movement events), updates the window state, and calls the corresponding functions (which we can set via callback methods)`
		}
	}
	void cleanup() {
		vkDestroyInstance(instance, nullptr); //equivelant of WM_DESTROY in windowsGDI

		glfwDestroyWindow(window);
		glfwTerminate();
	}
	//TODO: 
	// 1. clean up code (done)
	// 2. set up the physical and logical devices.
	// 3. create a swap chain to present images to the screen
	// 4. create graphics pipeline to render the triangle (goal)
	// 5. create render passes, commandbuffers and framebuffers
	// 6. draw the triangle

};

int main() {
	HelloTriangleApplication app; // 

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}