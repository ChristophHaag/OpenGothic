#pragma once

#include <Tempest/Device>
#include <Tempest/Matrix4x4>

#include "graphics/dynamic/painter3d.h"

#include "meshobjects.h"
#include "sceneglobals.h"

class RendererStorage;
class Item;

class InventoryRenderer {
  public:
    InventoryRenderer(const RendererStorage& storage);

    void draw(Tempest::Encoder<Tempest::CommandBuffer>& cmd, uint8_t fId);

    void reset();
    void drawItem(int x, int y, int w, int h, const Item &item);

  private:
    struct PerFrame {
      Tempest::Matrix4x4 mvp;
      };

    struct Itm {
      MeshObjects::Mesh mesh;
      int x=0,y=0,w=0,h=0;
      };

    SceneGlobals           scene;
    MeshObjects            itmGroup;
    std::vector<Itm>       items;
  };

