#include <fstream>
#include <sstream>
#include <iostream>

#include "file.h"
#include "container.h"
#include "connection.h"


namespace File
{
	namespace
	{
		constexpr int NODE_LINE_TYPE = 0;
		constexpr int CONNECTION_LINE_TYPE = 1;
		constexpr int CONNECTION_DIRECTION_FORWARD = 0;
	}

	bool LoadNodes(const std::string& filename, int& line_count, int& node_count, int& connection_count)
	{
		std::ifstream file("scriptfiles/" + filename);

		if (!file.good())
		{
			return false;
		}

		Container::LockExclusive();

		std::string buffer;
		auto type = 0, id = 0, id2 = 0, direction = 0, ignore = 0;
		auto x = 0.0f, y = 0.0f, z = 0.0f;

		while (std::getline(file, buffer))
		{
			std::istringstream input(buffer);

			input >> type;

			switch (type)
			{
			case NODE_LINE_TYPE:
			{
				input >> x >> y >> z >> ignore >> id;

				if (Container::Nodes::Add(id, x, y, z))
				{
					node_count++;
				}

				break;
			}

			case CONNECTION_LINE_TYPE:
			{
				input >> id >> id2 >> direction;

				if (direction != 2 && Container::Connections::Add(id, id2) != INVALID_CONNECTION_ID)
				{
					connection_count++;
				}

				break;
			}
			}

			line_count++;
		}

		Container::UnlockExclusive();

		file.close();
		return true;
	}


	bool SaveNodes(const std::string& filename)
	{
		std::ofstream file("scriptfiles/" + filename);

		if (!file.good())
		{
			return false;
		}

		Container::LockShared();
		const auto nodes = Container::Nodes::GetAll();
		const auto connections = Container::Connections::GetAll();
		Container::UnlockShared();

		for (const auto& [node_id, node_ptr] : nodes)
		{
			(void)node_id;

			if (!node_ptr->isSetForDeletion())
			{
				file << NODE_LINE_TYPE << " " << node_ptr->getX() << " " << node_ptr->getY() << " " << node_ptr->getZ() << " " << -1 << " " << node_ptr->getID() << '\n';
			}
		}

		for (const auto& [connection_id, connection_ptr] : connections)
		{
			(void)connection_id;
			file << CONNECTION_LINE_TYPE << " " << connection_ptr->getSource()->getID() << " " << connection_ptr->getTarget()->getID() << " " << CONNECTION_DIRECTION_FORWARD << '\n';
		}

		file.close();
		return true;
	}
};
