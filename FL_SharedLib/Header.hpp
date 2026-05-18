#pragma once
#include <cstdint>
#include <vector>
#include "NetUtils.hpp"
#include "PacketDataTypes.hpp"

//TODO: нужно как-то убрать сильную зависимость и знания всех использующих bitpacker от конкретных типов. Например, если я захочу изменить размерность size, то мне придётся 
// менять запись во всех файлах, проверяющих и использующих size. Предлагаю использовать using для создания типа headerSize. Также нужно под это дело как-то сделать так,
// чтобы все использующие headerSize использовали специальный метод для чтения этих данных из пакета, а не метод read_uint32_t
namespace sl::net {
	struct HeaderData {
		uint32_t size = 0;
		uint16_t sequenceNumber = 0;
		uint8_t type = 0;
		uint32_t token = 0;
	};

	struct Header {
	public:
		virtual bool write(std::vector<uint8_t>& out) const;
		virtual void read(const std::vector<uint8_t>& in, size_t& offset);
		void fillHeader(uint16_t sequenceNumber, PacketType type, uint32_t token, uint32_t dataSize);
		HeaderData getData() const { return data; }
	private:
		HeaderData data;
	};
}