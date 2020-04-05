#ifndef FLUXIONS_ENTITY_COMPONENT_SYSTEM_HPP
#define FLUXIONS_ENTITY_COMPONENT_SYSTEM_HPP

#include <cstdint>
#include <string>

namespace Fluxions {
	struct Entity {
		uint64_t entity_id;
		std::string desc;
	};

	struct Component {
		uint64_t component_id;
		std::string name;
		std::string desc;
		std::string tableName;
	};

	struct EntityComponent {
		uint64_t entity_id;
		uint64_t component_id;
	};
}

#endif
