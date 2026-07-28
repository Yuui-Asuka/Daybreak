#include "Scene.h"

#include "../Entity/Entity.h"


namespace Daybreak
{


    void Scene::AddEntity(
        Entity* entity
    )
    {

        m_Entities.push_back(
            entity
        );

    }



    const std::vector<Entity*>&
        Scene::GetEntities() const
    {

        return m_Entities;

    }


}