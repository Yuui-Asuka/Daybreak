#pragma once

#include <vector>

#include "../Entity/Entity.h"


namespace Daybreak
{

    class Scene
    {

    public:

        void AddEntity(Entity* entity);


        const std::vector<Entity*>& GetEntities() const;


    private:

        std::vector<Entity*> m_Entities;

    };


}