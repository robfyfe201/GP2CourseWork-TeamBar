#ifndef GameObject_h
#define GameObject_h

#include <string>
#include <vector>


class Component;
class Transform;
class Mesh;
class BaseMaterial;
class Camera;
class Light;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void init();

	void update();

	void render();

	void destroy();

	void addComponent(Component * component);
	void setName(const std::string& name);
	const std::string& getName();

	void setTransform(Transform * transform);
	void setMesh(Mesh * mesh);
	void setMaterial(BaseMaterial * material);
	void setCamera(Camera * camera);
	void setLight(Light * light);

	void addChild(GameObject * obj);
	void setParent(GameObject *parent);

	GameObject *getParent();

	Transform * getTransform();
	Mesh *getMesh();
	BaseMaterial *getMaterial();
	Camera * getCamera();
	Light* getLight();

	int getChildCount();
	GameObject * getChild(int index);

	Component * getComponentByType(const std::string& type);
	Component * getComponentByName(const std::string& name);
protected:
private:
	std::vector<Component*> m_Components;
	std::vector<GameObject*> m_Children;
	std::string m_Name;

	Transform *m_Transform;
	Mesh *m_Mesh;
	BaseMaterial *m_Material;
	Camera *m_Camera;
	Light *m_Light;

	GameObject *m_Parent;
};


#endif