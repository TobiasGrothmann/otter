#include "Voronoi.hpp"
#define JC_VORONOI_IMPLEMENTATION
#define JCV_REAL_TYPE double
#include "jc_voronoi/jc_voronoi.h"

using namespace std;

namespace otter {

Voronoi::Voronoi(const vector<Vec2>& points, shared_ptr<Rectangle> clipBounds)
    : points(points)
    , clipBounds(clipBounds)
{
    jcv_point* points_v = new jcv_point[points.size()] {};
    for (VINDEX(points) i = 0; i < points.size(); i++)
    {
        points_v[i] = { points[i].x, points[i].y };
    }
    
    jcv_diagram diagram;
    memset(&diagram, 0, sizeof(jcv_diagram));
    
    jcv_rect* clipBounds_v = nullptr;
    if (clipBounds != nullptr)
    {
        clipBounds_v = new jcv_rect {
            { clipBounds->getBottomLeft().x, clipBounds->getBottomLeft().y },
            { clipBounds->getTopRight().x, clipBounds->getTopRight().y }
        };
    }
    jcv_diagram_generate((int)points.size(), points_v, clipBounds_v, 0, &diagram);
    delete clipBounds_v;
    delete[] points_v;
    
    // get edges
    const jcv_edge* edge = jcv_diagram_get_edges(&diagram);
    while (edge)
    {
        Line line = Line(Vec2(edge->pos[0].x, edge->pos[0].y), Vec2(edge->pos[1].x, edge->pos[1].y));
        edges.push_back(line);
        edge = jcv_diagram_get_next_edge(edge);
    }
    delete edge;
    
    // get cells
    const jcv_site* sites = jcv_diagram_get_sites(&diagram);
    for( int i = 0; i < diagram.numsites; ++i )
    {
        const jcv_site* site = &sites[i];
        const Vec2 point = Vec2(site->p.x, site->p.y);
        
        vector<Line> edges = {};
        vector<Vec2> vertices = {};
        const jcv_graphedge* e = site->edges;
        while (e)
        {
            edges.push_back({
                Vec2(e->pos[0].x, e->pos[0].y),
                Vec2(e->pos[1].x, e->pos[1].y)
            });
            vertices.push_back(Vec2(e->pos[0].x, e->pos[0].y));
            e = e->next;
        }
        
        cells.push_back(VoronoiCell(point, edges, vertices));
    }
    
    jcv_diagram_free(&diagram);
}

}
