#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid.lib")

#include "dxd/buffer.h"
#include "dxd/vertex.h"

#include "dxd/mesh.h"
#include "dxd/game_object.h"
#include "dxd/renderer.h"
#include "dxd/shader.h"
#include "dxd/window.h"
#include "dxd/keyboard.h"
#include "dxd/mouse.h"

#include "dxd/component.h"
#include "dxd/mesh_renderer_component.h"
#include "dxd/transform_component.h"
#include "dxd/camera_component.h"
#include "dxd/collider_component.h"
#include "dxd/rigidbody_component.h"

#include "dxd/mesh_loader.h"

#include "dxd/imgui_manager.h"

#include "dxd/log.h"