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
	VkImage m_textureImage = nullptr;
	VkDeviceMemory m_textureImageMemory = nullptr;
	VkImageView m_textureImageView = nullptr;
	VkSampler m_textureSampler = nullptr;

	void freeTrexture();
};

/**
 * @brief Class Descriptors.
 */
class Descriptors {
public:
	Descriptors(const Descriptors &) = delete;
	Descriptors(Descriptors &&) = delete;
	Descriptors &operator=(const Descriptors &) = delete;
	Descriptors &operator=(Descriptors &&) = delete;
	/**
	 * @brief Destructor.
	 */
	~Descriptors();

	/**
	 * @brief Singleton's accessor.
	 * @return The instance of this object.
	 */
	static Descriptors &get() {
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

	TextureData &getTextureData(uint32_t iIndex);

	void bindTextureImage(uint32_t iIndex);

	void unregisterTexture(uint32_t iIndex);

	void setUniformData(const void *iData, size_t iSize) const;

	void resetTextureBind();
	void commitTextureBind(size_t iCurrentFrame);
	void textureBind(uint32_t iIndex);

	[[nodiscard]] VkDescriptorPool getDescriptorPool() const { return m_descriptorPool; }

	VkDescriptorSetLayout *getDescriptorSetLayout() { return &m_descriptorSetLayout; }
	VkDescriptorSet *getDescriptorSet(const uint32_t iFrame) { return &m_descriptorSets[iFrame]; }

	void createImguiDescriptorPool();
	[[nodiscard]] VkDescriptorPool getImguiDescriptorPool() const { return m_imguiDescriptorPool; }

private:
	/**
	 * @brief Default Constructor.
	 */
	Descriptors();
	/// The list of texture bindings.
	std::unordered_map<uint32_t, TextureData> m_textures;
	uint32_t nextId = 0;

	/// The descriptor set layout.
	VkDescriptorSetLayout m_descriptorSetLayout{nullptr};
	/// The descripto pool.
	VkDescriptorPool m_descriptorPool{nullptr};

	VkDescriptorPool m_imguiDescriptorPool{nullptr};
	/// List of descripto
	std::vector<VkDescriptorSet> m_descriptorSets;
	std::vector<VkBuffer> m_uniformBuffers;
	std::vector<VkDeviceMemory> m_uniformBuffersMemory;
	std::vector<void *> m_uniformBuffersMapped;
	uint32_t m_uniformSize = 0;
	uint32_t m_bindedTexture = 0;
	std::vector<uint32_t> m_textureBind;

	void updateDescriptor(size_t iFrame);
};

}// internal
