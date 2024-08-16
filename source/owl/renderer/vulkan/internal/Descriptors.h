/**
 * @file Descriptors.h
 * @author Silmaen
 * @date 13/03/2024
 * Copyright © 2024 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <vulkan/vulkan.h>

namespace owl::renderer::vulkan::internal {

/**
 * @brief Data for Texture description.
 */
struct TextureData {
	VkImage textureImage = nullptr;
	VkDeviceMemory textureImageMemory = nullptr;
	VkImageView textureImageView = nullptr;
	VkSampler textureSampler = nullptr;
	VkDescriptorSet textureDescriptorSet = nullptr;
	VkDescriptorSetLayout textureDescriptorSetLayout = nullptr;

	void freeTrexture();
	void createDescriptorSet();
	void createView();
	void createSampler();
	void createImage(const math::vec2ui& iDimensions);
};

/**
 * @brief Class Descriptors.
 */
class Descriptors {
public:
	Descriptors(const Descriptors&) = delete;
	Descriptors(Descriptors&&) = delete;
	Descriptors& operator=(const Descriptors&) = delete;
	Descriptors& operator=(Descriptors&&) = delete;
	/**
	 * @brief Destructor.
	 */
	~Descriptors();

	/**
	 * @brief Singleton's accessor.
	 * @return The instance of this object.
	 */
	static Descriptors& get() {
		static Descriptors instance;
		return instance;
	}

	/**
	 * @brief Create the descriptor sets
	 */
	void createDescriptors();

	/**
	 * @brief Update the descriptor sets.
	 */
	void updateDescriptors();

	/**
	 * @brief Destroy everything.
	 */
	void release();

	void registerUniform(uint32_t iSize);

	/**
	 * @brief Register new texture data in the
	 * @return Id of the created texture.
	 */
	uint32_t registerNewTexture();

	TextureData& getTextureData(uint32_t iIndex);

	void bindTextureImage(uint32_t iIndex);

	void unregisterTexture(uint32_t iIndex);

	[[nodiscard]] bool isTextureRegistered(uint32_t iIndex) const;

	void setUniformData(const void* iData, size_t iSize) const;

	void resetTextureBind();
	void commitTextureBind(size_t iCurrentFrame);
	void textureBind(uint32_t iIndex);

	[[nodiscard]] VkDescriptorPool getDescriptorPool() const { return m_descriptorPool; }

	VkDescriptorSetLayout* getDescriptorSetLayout() { return &m_descriptorSetLayout; }
	VkDescriptorSet* getDescriptorSet(const uint32_t iFrame) { return &m_descriptorSets[iFrame]; }

	void createImguiDescriptorPool();
	[[nodiscard]] VkDescriptorPool getImguiDescriptorPool() const { return m_imguiDescriptorPool; }
	void createSingleImageDescriptorPool();

	[[nodiscard]] VkDescriptorPool getSingleImageDescriptorPool() {
		if (!m_singleImageDescriptorPool)
			createSingleImageDescriptorPool();
		return m_singleImageDescriptorPool;
	}

private:
	/**
	 * @brief Default Constructor.
	 */
	Descriptors();

	struct TextureList {
		using tex = shared<TextureData>;
		using elem = std::pair<uint32_t, tex>;
		using list = std::vector<elem>;
		list textures;
		uint32_t nextId = 0;
		[[nodiscard]] bool empty() const { return textures.empty(); }
		void clear() {
			textures.clear();
			textures.shrink_to_fit();
		}
		list::iterator begin() { return textures.begin(); }
		list::iterator end() { return textures.begin(); }
		[[nodiscard]] list::const_iterator begin() const { return textures.begin(); }
		[[nodiscard]] list::const_iterator end() const { return textures.begin(); }
		[[nodiscard]] bool contains(uint32_t iIndex) const;
		uint32_t registerNewTexture();
		void unregisterTexture(uint32_t iIndex);
		tex getTextureData(uint32_t iIndex);
	};

	/// The list of texture bindings.
	TextureList m_textures;

	/// The descriptor set layout.
	VkDescriptorSetLayout m_descriptorSetLayout{nullptr};
	/// The main descriptor pool.
	VkDescriptorPool m_descriptorPool{nullptr};
	/// The descriptor pool for Imgui.
	VkDescriptorPool m_imguiDescriptorPool{nullptr};
	/// The descriptor pool for the single images.
	VkDescriptorPool m_singleImageDescriptorPool{nullptr};
	/// List of descripto
	std::vector<VkDescriptorSet> m_descriptorSets;
	std::vector<VkBuffer> m_uniformBuffers;
	std::vector<VkDeviceMemory> m_uniformBuffersMemory;
	std::vector<void*> m_uniformBuffersMapped;
	uint32_t m_uniformSize = 0;
	uint32_t m_bindedTexture = 0;
	std::vector<uint32_t> m_textureBind;

	void updateDescriptor(size_t iFrame);
};

}// namespace owl::renderer::vulkan::internal
