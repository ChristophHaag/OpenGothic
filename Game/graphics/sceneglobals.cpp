#include "sceneglobals.h"

#include "rendererstorage.h"
#include "objectsbucket.h"

#include <cassert>

SceneGlobals::SceneGlobals(const RendererStorage& storage)
  :storage(storage) {
  uboGlobal.lightDir={1,1,-1};
  uboGlobal.lightDir/=uboGlobal.lightDir.manhattanLength();

  uboGlobal.modelView.identity();
  uboGlobal.shadowView.identity();

  sun.setDir(1,-1,1);

  for(uint8_t fId=0; fId<Resources::MaxFramesInFlight; ++fId)
    for(uint8_t lay=0; lay<Resources::ShadowLayers; ++lay) {
      uboGlobalPf[fId][lay] = storage.device.ubo<UboGlobal>(nullptr,1);
      }
  }

SceneGlobals::~SceneGlobals() {
  }

void SceneGlobals::setModelView(const Tempest::Matrix4x4& m, const Tempest::Matrix4x4* sh, size_t shCount) {
  assert(shCount==2);

  uboGlobal.modelView  = m;
  uboGlobal.shadowView = sh[0];
  shadowView1          = sh[1];
  }

void SceneGlobals::commitUbo(uint8_t fId) {
  auto  d = sun.dir();
  auto& c = sun.color();

  uboGlobal.lightDir = {-d.x,-d.y,-d.z};
  uboGlobal.lightCl  = {c.x,c.y,c.z,0.f};
  uboGlobal.lightAmb = {ambient.x,ambient.y,ambient.z,0.f};

  auto ubo2 = uboGlobal;
  ubo2.shadowView = shadowView1;

  uboGlobalPf[fId][0].update(&uboGlobal,0,1);
  uboGlobalPf[fId][1].update(&ubo2,     0,1);
  }

void SceneGlobals::setShadowmMap(const Tempest::Texture2d& tex) {
  shadowMap            = &tex;
  uboGlobal.shadowSize = float(tex.w());
  }

const Tempest::Matrix4x4& SceneGlobals::modelView() const {
  return uboGlobal.modelView;
  }
