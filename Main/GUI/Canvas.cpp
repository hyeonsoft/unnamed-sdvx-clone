#include "stdafx.h"
#include "GUI.hpp"
#include "GUIRenderer.hpp"

// Canvas

Canvas::~Canvas()
{
	for(auto& c : m_children)
	{
		delete c;
	}
}
void Canvas::Render(GUIRenderData rd)
{
	for(auto& c : m_children)
	{
		c->Render(rd);
	}
}
void Canvas::Add(GUIElement element)
{
	bool found = false;
	for(auto it = m_children.begin(); it != m_children.end(); it++)
	{
		if((*it)->element == element)
			return; // Already exists
	}

	m_children.AddUnique(CreateSlot<Canvas::Slot>(element));
	m_SortChildren();
}
void Canvas::Remove(GUIElement element)
{
	for(auto it = m_children.begin(); it != m_children.end();)
	{
		if((*it)->element == element)
			m_children.erase(it);
		else
			it++;
	}
}
const Vector<Canvas::Slot*>& Canvas::GetChildren()
{
	return m_children;
}
void Canvas::m_OnZOrderChanged(GUISlotBase* slot)
{
	m_SortChildren();
}
void Canvas::m_SortChildren()
{
	m_children.Sort([](const GUISlotBase* l, const GUISlotBase* r)
	{
		return l->GetZOrder() < r->GetZOrder();
	});
}

void Canvas::Slot::Render(GUIRenderData rd)
{
	rd.isSizeFixed = !autoSize;

	// Apply anchor and offset to get the canvas rectangle
	Rect base = anchor.Apply(rd.area);
	base.pos += offset.pos;
	base.size += offset.size;
	rd.guiRenderer->SetScissorRect(base);

	rd.area = base;

	element->Render(rd);
}