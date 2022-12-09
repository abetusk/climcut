// 
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "mcut/mcut.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include <errno.h>

#include <vector>

#include <Eigen/Core>
#include <igl/read_triangle_mesh.h>
#include <igl/writeOBJ.h>

std::vector< std::vector<double> > g_vtx_group;
std::vector< std::vector<uint32_t> > g_face_group;

void debug_print(int g) {
  int i, j, k;

  if ((g<0) || (g>=g_vtx_group.size())) { g = 0; }
  if (g>=g_vtx_group.size()) { return; }

  for (i=0; i<g_vtx_group[g].size(); i+=3) {
    printf("v %f %f %f\n",
      g_vtx_group[g][i+0],
      g_vtx_group[g][i+1],
      g_vtx_group[g][i+2]);
  }

  for (i=0; i<g_face_group[g].size(); i+=3) {
    printf("f %i %i %i\n",
      (int)(g_face_group[g][i+0]+1),
      (int)(g_face_group[g][i+1]+1),
      (int)(g_face_group[g][i+2]+1));
  }

}

void print_OFF_g(FILE *ofp, int g) {
  int i;

  fprintf(ofp, "OFF\n");
  fprintf(ofp, "%i %i %i\n",
      (int)(g_vtx_group[g].size()/3),
      (int)(g_face_group[g].size()/3),
      0);

  for (i=0; i<g_vtx_group[g].size(); i+=3) {
    fprintf(ofp, "%f %f %f\n",
        g_vtx_group[g][i+0],
        g_vtx_group[g][i+1],
        g_vtx_group[g][i+2]);
  }

  for (i=0; i<g_face_group[g].size(); i+=3) {
    fprintf(ofp, "%i %i %i %i\n",
        3,
        (int)(g_face_group[g][i+0]),
        (int)(g_face_group[g][i+1]),
        (int)(g_face_group[g][i+2]));
  }

}

void print_OBJ_g(FILE *ofp, int g) {
  int i;

  for (i=0; i<g_vtx_group[g].size(); i+=3) {
    fprintf(ofp, "v %f %f %f\n",
      g_vtx_group[g][i+0],
      g_vtx_group[g][i+1],
      g_vtx_group[g][i+2]);
  }

  for (i=0; i<g_face_group[g].size(); i+=3) {
    fprintf(ofp, "f %i %i %i\n",
      (int)(g_face_group[g][i+0]+1),
      (int)(g_face_group[g][i+1]+1),
      (int)(g_face_group[g][i+2]+1));
  }

}

void print_GP_g(FILE *ofp, int g) {
  int i;
  int v0, v1, v2;

  for (i=0; i<g_face_group[g].size(); i+=3) {
    v0 = (int)(3*g_face_group[g][i+0]);
    v1 = (int)(3*g_face_group[g][i+1]);
    v2 = (int)(3*g_face_group[g][i+2]);

    printf("%f %f %f\n",
        g_vtx_group[g][v0+0],
        g_vtx_group[g][v0+1],
        g_vtx_group[g][v0+2]);

    printf("%f %f %f\n",
        g_vtx_group[g][v1+0],
        g_vtx_group[g][v1+1],
        g_vtx_group[g][v1+2]);

    printf("%f %f %f\n",
        g_vtx_group[g][v2+0],
        g_vtx_group[g][v2+1],
        g_vtx_group[g][v2+2]);

    printf("\n\n");
  }

}

void print_STL_g(FILE *ofp, int g) {
  int i;
  int v0, v1, v2;

  fprintf(ofp, "solid\n");
  for (i=0; i<g_face_group[g].size(); i+=3) {
    v0 = (int)(3*g_face_group[g][i+0]);
    v1 = (int)(3*g_face_group[g][i+1]);
    v2 = (int)(3*g_face_group[g][i+2]);

    fprintf(ofp, "facet normal %f %f %f\n", 0.0, 0.0, 0.0);
    fprintf(ofp, "  outer loop\n");
    fprintf(ofp, "    vertex %f %f %f\n",
        g_vtx_group[g][v0+0],
        g_vtx_group[g][v0+1],
        g_vtx_group[g][v0+2]); 
    fprintf(ofp, "    vertex %f %f %f\n",
        g_vtx_group[g][v1+0],
        g_vtx_group[g][v1+1],
        g_vtx_group[g][v1+2]); 
    fprintf(ofp, "    vertex %f %f %f\n",
        g_vtx_group[g][v2+0],
        g_vtx_group[g][v2+1],
        g_vtx_group[g][v2+2]); 
    fprintf(ofp, "  endloop\n");
    fprintf(ofp, "endfacet\n");
  }

  fprintf(ofp, "endsolid\n");
}

int print_fn(std::string &ofn, std::string &oft, int g) {
  std::string fn, sfx;
  FILE *ofp=NULL;
  char buf[128];

  if (ofn == "-") { ofp = stdout; }

  if ((oft == "gnuplot") || (oft == "GP") || (oft == "gp")) {
    sfx = ".gp";
  }
  else if ((oft == "off") || (oft == "OFF")) {
    sfx = ".off";
  }
  else if ((oft == "obj") || (oft == "OBJ")) {
    sfx = ".obj";
  }
  else if ((oft == "stl") || (oft == "STL")) {
    //todo
    sfx  = ".stl";
  }
  else { return -1; }

  if (g>=0) {
    if (ofp != stdout) {
      ofp = fopen(ofn.c_str(), "w");
      if (ofp == NULL) { return -1; }
    }

    if      (sfx == ".gp")  { print_GP_g(ofp, g); }
    else if (sfx == ".off") { print_OFF_g(ofp, g); }
    else if (sfx == ".obj") { print_OBJ_g(ofp, g); }
    else if (sfx == ".stl") { print_STL_g(ofp, g); }

    if (ofp != stdout) { fclose(ofp); }

    return 0;
  }

  for (g=0; g<g_vtx_group.size(); g++) {

    if (ofp != stdout) {
      snprintf(buf, 64, "%02i", g);
      fn = ofn;
      fn += buf;
      fn += sfx;

      ofp = fopen(fn.c_str(), "w");
      if (ofp == NULL) { return -1; }
    }

    if      (sfx == ".gp")  { print_GP_g(ofp, g); }
    else if (sfx == ".off") { print_OFF_g(ofp, g); }
    else if (sfx == ".obj") { print_OBJ_g(ofp, g); }
    else if (sfx == ".stl") { print_STL_g(ofp, g); }

    if (ofp != stdout) { fclose(ofp); }
  }

  return 0;
}

extern "C" { 
  int climcut_hello(void) {
    printf("hello\n");
    return 0;
  }

  int mcutop( double *subj_vtx, int subj_n_vtx, int *subj_face, int subj_n_face,
              double *clip_vtx, int clip_n_vtx, int *clip_face, int clip_n_face,
              int op);
}

//#define CC_EMSCRIPTEN
#ifdef CC_EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

emscripten::val mcut_n() {
  return emscripten::val(g_vtx_group.size());
}

emscripten::val mcut_v(int g) {
  return emscripten::val(emscripten::typed_memory_view( g_vtx_group[g].size(), ((double *)(&(g_vtx_group[g][0]))) ) );
}

emscripten::val mcut_f(int g) {
  return emscripten::val(emscripten::typed_memory_view( g_face_group[g].size(), ((uint32_t *)(&(g_face_group[g][0]))) ) );
}

EMSCRIPTEN_BINDINGS(mcut_js_function_bindings) {
  emscripten::function("mcut_n", &mcut_n);
  emscripten::function("mcut_v", &mcut_v);
  emscripten::function("mcut_f", &mcut_f);
}

#endif


void exit_err(McResult err) {
  fprintf(stderr, "error: %i, exiting\n", (int)err);
  exit(-1);
}

struct InputMesh {
  std::vector<std::vector<double>> V;
  std::vector<std::vector<int>> F;

  std::string fpath;
  std::vector<uint32_t> faceSizesArray;
  std::vector<uint32_t> faceIndicesArray;
  std::vector<double> vertexCoordsArray;
};


int mcutop( double *subj_vtx, int subj_n_vtx, int *subj_face, int subj_n_face,
            double *clip_vtx, int clip_n_vtx, int *clip_face, int clip_n_face,
            int op) {
  int i;
  InputMesh srcMesh,
            cutMesh;
  McContext ctx;
  McResult err;

  McFlags flags = MC_DISPATCH_FILTER_ALL;

  uint32_t n_component;
  int patch_idx=0;

  std::vector<int32_t> src_face_size,
                       cut_face_size;

  std::vector< std::vector< double > > _res_vv;
  std::vector< double > _v;

  // a - b (0)
  // b - a (1)
  // intersection (3)
  // union (2)
  //
  if      (op == 0) { flags = MC_DISPATCH_FILTER_FRAGMENT_SEALING_INSIDE | MC_DISPATCH_FILTER_FRAGMENT_LOCATION_ABOVE; }
  else if (op == 1) { flags = MC_DISPATCH_FILTER_FRAGMENT_SEALING_OUTSIDE | MC_DISPATCH_FILTER_FRAGMENT_LOCATION_BELOW; }
  else if (op == 3) { flags = MC_DISPATCH_FILTER_FRAGMENT_SEALING_INSIDE | MC_DISPATCH_FILTER_FRAGMENT_LOCATION_BELOW; }
  else              { flags = MC_DISPATCH_FILTER_FRAGMENT_SEALING_OUTSIDE | MC_DISPATCH_FILTER_FRAGMENT_LOCATION_ABOVE; }

  ctx = MC_NULL_HANDLE;
  err = mcCreateContext(&ctx, 0);
  if (err != MC_NO_ERROR) { return -1; }

  err = mcDebugMessageControl(ctx, MC_DEBUG_SOURCE_ALL, MC_DEBUG_TYPE_ALL, MC_DEBUG_SEVERITY_ALL, false);
  if (err != MC_NO_ERROR) { return -1; }

  for (i=0; i<subj_n_face; i++) { src_face_size.push_back(3); }
  for (i=0; i<clip_n_face; i++) { cut_face_size.push_back(3); }

  err = mcDispatch( ctx,
                    MC_DISPATCH_VERTEX_ARRAY_DOUBLE |
                      MC_DISPATCH_ENFORCE_GENERAL_POSITION |
                      flags,
                    subj_vtx,
                    (const uint32_t *)subj_face,
                    (const uint32_t *)(&(src_face_size[0])),
                    subj_n_vtx,
                    subj_n_face,

                    clip_vtx,
                    (const uint32_t *)clip_face,
                    (const uint32_t *)(&(cut_face_size[0])) ,
                    clip_n_vtx,
                    clip_n_face );
  if (err != MC_NO_ERROR) { return -1; }

  err = mcGetConnectedComponents(ctx, MC_CONNECTED_COMPONENT_TYPE_FRAGMENT, 0, NULL, &n_component);
  if (err != MC_NO_ERROR) { return -2; }

  if (n_component == 0) { return 0; }

  std::vector<McConnectedComponent> connectedComponents(n_component, MC_NULL_HANDLE);
  connectedComponents.resize(n_component);
  err = mcGetConnectedComponents( ctx,
                                  MC_CONNECTED_COMPONENT_TYPE_FRAGMENT,
                                  (uint32_t)connectedComponents.size(),
                                  connectedComponents.data(),
                                  NULL);
  if (err != MC_NO_ERROR) { return -3; }

  g_vtx_group.clear();
  g_face_group.clear();

  for (patch_idx = 0; patch_idx < n_component; patch_idx++) {
    McConnectedComponent connComp = connectedComponents[patch_idx];
    uint64_t numBytes = 0;
    err = mcGetConnectedComponentData(ctx, connComp, MC_CONNECTED_COMPONENT_DATA_VERTEX_DOUBLE, 0, NULL, &numBytes);
    if (err != MC_NO_ERROR) { return -1; }

    uint32_t ccVertexCount = (uint32_t)(numBytes / (sizeof(double) * 3));
    std::vector<double> ccVertices((uint64_t)ccVertexCount * 3u, 0);
    err = mcGetConnectedComponentData(ctx, connComp, MC_CONNECTED_COMPONENT_DATA_VERTEX_DOUBLE, numBytes, (void*)ccVertices.data(), NULL);
    if (err != MC_NO_ERROR) { return -1; }

    //--

    numBytes = 0;

    err = mcGetConnectedComponentData(ctx, connComp, MC_CONNECTED_COMPONENT_DATA_FACE_TRIANGULATION, 0, NULL, &numBytes);
    if (err != MC_NO_ERROR) { return -1; }

    std::vector<uint32_t> ccFaceIndices(numBytes / sizeof(uint32_t), 0);
    err = mcGetConnectedComponentData(ctx, connComp, MC_CONNECTED_COMPONENT_DATA_FACE_TRIANGULATION, numBytes, ccFaceIndices.data(), NULL);
    if (err != MC_NO_ERROR) { return -1; }

    std::vector<uint32_t> faceSizes(ccFaceIndices.size() / 3, 3);
    const uint32_t ccFaceCount = static_cast<uint32_t>(faceSizes.size());

    McPatchLocation patchLocation = (McPatchLocation)0;

    err = mcGetConnectedComponentData(ctx, connComp, MC_CONNECTED_COMPONENT_DATA_PATCH_LOCATION, sizeof(McPatchLocation), &patchLocation, NULL);
    if (err != MC_NO_ERROR) { return -1; }

    McFragmentLocation fragmentLocation = (McFragmentLocation)0;
    err = mcGetConnectedComponentData(  ctx,
                                        connComp,
                                        MC_CONNECTED_COMPONENT_DATA_FRAGMENT_LOCATION,
                                        sizeof(McFragmentLocation), &fragmentLocation,
                                        NULL);
    if (err != MC_NO_ERROR) { return -1; }

    uint64_t faceVertexOffsetBase = 0;

    // reverse winding of triangle mesh if need be
    //
    for (uint32_t f = 0; f < ccFaceCount; ++f) {
      bool reverseWindingOrder = (fragmentLocation == MC_FRAGMENT_LOCATION_BELOW) && (patchLocation == MC_PATCH_LOCATION_OUTSIDE);
      int faceSize = faceSizes.at(f);
      if (!reverseWindingOrder) {
        faceVertexOffsetBase += faceSize;
        continue;
      }

      for (int v=0; v<(faceSize/2); v++) {
        uint64_t idx = (uint64_t)faceVertexOffsetBase;
        int t = ccFaceIndices[idx+v];
        ccFaceIndices[idx+v] = ccFaceIndices[idx+faceSize-v-1];
        ccFaceIndices[idx+faceSize-v-1] = t;
      }

      faceVertexOffsetBase += faceSize;
    }

    // populate our global data structure for access outside (js)
    //
    g_vtx_group.push_back( ccVertices );
    g_face_group.push_back( ccFaceIndices );
  }

  return 0;
}

int loadMeshOFF(InputMesh &mesh, const char *fn) {
  bool meshLoaded = false;

  mesh.fpath = fn;
  meshLoaded = igl::read_triangle_mesh(mesh.fpath, mesh.V, mesh.F);

  if (!meshLoaded) { return -1; }

  for (int i = 0; i < (int)mesh.V.size(); ++i) {
    const std::vector<double>& v = mesh.V[i];
    if (v.size()!=3) { return -2; }
    mesh.vertexCoordsArray.push_back(v[0]);
    mesh.vertexCoordsArray.push_back(v[1]);
    mesh.vertexCoordsArray.push_back(v[2]);
  }

  for (int i = 0; i < (int)mesh.F.size(); ++i) {
    const std::vector<int>& f = mesh.F[i];
    for (int j = 0; j < (int)f.size(); ++j) {
      mesh.faceIndicesArray.push_back(f[j]);
    }
    mesh.faceSizesArray.push_back((uint32_t)f.size());
  }

  return 0;
}

void show_help(FILE *fp) {
  fprintf(fp, "\nusage:\n\n    climcut [-h] [-v] [-s subject] [-c clip] [-t op] [-o output]\n");
  fprintf(fp, "\n");
  fprintf(fp, "  [-s subj]    subject file\n");
  fprintf(fp, "  [-c clip]    clip file\n");
  fprintf(fp, "  [-t op]      operation (0 - a-b, 1 - b-a, 2 - union, 3 - intersection)\n");
  fprintf(fp, "  [-o ofn]     output filename (base filename in the case of outputting all connected components/groups)\n");
  fprintf(fp, "  [-O fmt]     output format (stl, off, obj, gp)\n");
  fprintf(fp, "  [-g group#]  group number (<0 for all, default 0)\n");
  fprintf(fp, "  [-v]         version\n");
  fprintf(fp, "  [-h]         help (this screen)\n");
  fprintf(fp, "\n");
}

int main(int argc, char **argv) {
  FILE *ofp=stdout;
  int _r = 0, ch=0;

  InputMesh srcMesh, cutMesh;
  bool meshLoaded = false;

  uint32_t n_component;

  std::string subj_fn, clip_fn, out_fn = "-";
  int op_idx = 2;
  int patch_idx=0;

  McFlags flags = MC_DISPATCH_FILTER_ALL;
  McFlags flag_opts[4];

  // a - b
  //
  flag_opts[0] = MC_DISPATCH_FILTER_FRAGMENT_SEALING_INSIDE | MC_DISPATCH_FILTER_FRAGMENT_LOCATION_ABOVE;

  // b - a
  //
  flag_opts[1] = MC_DISPATCH_FILTER_FRAGMENT_SEALING_OUTSIDE | MC_DISPATCH_FILTER_FRAGMENT_LOCATION_BELOW;

  // union
  //
  flag_opts[2] = MC_DISPATCH_FILTER_FRAGMENT_SEALING_OUTSIDE | MC_DISPATCH_FILTER_FRAGMENT_LOCATION_ABOVE;

  // intersection
  //
  flag_opts[3] = MC_DISPATCH_FILTER_FRAGMENT_SEALING_INSIDE | MC_DISPATCH_FILTER_FRAGMENT_LOCATION_BELOW;

  flags = flag_opts[2];

  McContext ctx= MC_NULL_HANDLE;

  int group_idx = 0;
  std::string out_fmt = "obj";

  #ifdef CC_EMSCRIPTEN

  // if we're compiling for JS, just bag out so user can
  // use mcutop
  // 
  return 0;

  #endif


  //------

  while ((ch = getopt(argc, argv, "hvs:c:t:o:O:g:")) != -1) {
    switch(ch) {
      case 'h':
        show_help(stdout);
        exit(0);
      case 'v':
        show_help(stdout);
        exit(0);
      case 's':
        subj_fn = optarg;
        break;
      case 'c':
        clip_fn = optarg;
        break;
      case 't':
        op_idx = atoi(optarg);
        break;
      case 'g':
        group_idx = atoi(optarg);
        break;
      case 'O':
        out_fmt = optarg;
        break;
      case 'o':
        out_fn = optarg;
        break;
      default:
        fprintf(stderr, "bad argument\n");
        show_help(stderr);
        exit(1);
    }
  }

  if ((op_idx < 0)  || (op_idx >= 4)) {
    fprintf(stderr, "operation (-t) must be one in list\n");
    show_help(stderr);
    exit(-1);
  }

  if (subj_fn.size() == 0) {
    fprintf(stderr, "provide subject file\n");
    show_help(stderr);
    exit(-1);
  }

  if (clip_fn.size() == 0) {
    fprintf(stderr, "provide clip file\n");
    show_help(stderr);
    exit(-1);
  }

  flags = flag_opts[op_idx];

  //------

  McResult err = mcCreateContext(&ctx, 0);
  if (err != MC_NO_ERROR) { exit(-1); }

  err = mcDebugMessageControl(ctx, MC_DEBUG_SOURCE_ALL, MC_DEBUG_TYPE_ALL, MC_DEBUG_SEVERITY_ALL, false);

  _r = loadMeshOFF(srcMesh, subj_fn.c_str());
  if (_r < 0) { fprintf(stderr, "error loading subject file"); exit(-1); }

  _r = loadMeshOFF(cutMesh, clip_fn.c_str());
  if (_r < 0) { fprintf(stderr, "error loading clip file"); exit(-1); }

  mcutop( srcMesh.vertexCoordsArray.data(), srcMesh.vertexCoordsArray.size()/3,
          (int *)srcMesh.faceIndicesArray.data(), srcMesh.faceSizesArray.size(),
          cutMesh.vertexCoordsArray.data(), cutMesh.vertexCoordsArray.size()/3,
          (int *)cutMesh.faceIndicesArray.data(), cutMesh.faceSizesArray.size(),
          op_idx);

  std::string base_fn = "_out";
  int _ret=0;

  _ret = print_fn(base_fn, out_fmt, -1);
  if (_ret < 0) { exit(-1); }
}
