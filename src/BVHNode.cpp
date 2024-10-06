#include "Pch.h"
#include "BVHNode.h"

BVHNode::BVHNode(std::vector<HittablePtr> &objects, size_t start, size_t end)
{
    m_bbox = AABB::Empty;
    for (size_t objectIndex = start; objectIndex < end; ++objectIndex)
    {
        m_bbox = AABB(m_bbox, objects[objectIndex]->BoundingBox());
    }

    int axis = m_bbox.LongestAxis();

    std::function<bool(const HittablePtr &, const HittablePtr &)> comparators[] = {
        BoxXCompare,
        BoxYCompare,
        BoxZCompare};

    assert(axis < narrow_cast<int>(std::extent_v<decltype(comparators)>));
    auto comparator = comparators[axis];

    size_t objectSpan = end - start;
    switch (objectSpan)
    {
    case 1:
        m_left = m_right = objects[start];
        break;
    case 2:
        m_left = objects[start];
        m_right = objects[start + 1];
        break;
    default:
        std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

        size_t mid = start + objectSpan / 2;
        m_left = std::make_shared<BVHNode>(objects, start, mid);
        m_right = std::make_shared<BVHNode>(objects, mid, end);
    }

    m_bbox = AABB(m_left->BoundingBox(), m_right->BoundingBox());
}

bool BVHNode::Hit(const Ray &r, Interval rayT, HitRecord &rec) const
{
    if (!m_bbox.Hit(r, rayT))
    {
        return false;
    }

    bool hitLeft = m_left->Hit(r, rayT, rec);
    bool hitRight = m_right->Hit(r, Interval(rayT.min, hitLeft ? rec.t : rayT.max), rec);

    return hitLeft || hitRight;
}

bool BVHNode::BoxCompare(const HittablePtr &a, const HittablePtr &b, int axisIndex)
{
    Interval aAxisInterval = a->BoundingBox().AxisInterval(axisIndex);
    Interval bAxisInterval = b->BoundingBox().AxisInterval(axisIndex);
    return aAxisInterval.min < bAxisInterval.min;
}
