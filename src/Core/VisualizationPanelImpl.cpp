//----------------------------------------------------------------------
//	Copyright (C) 2020-present Matias N. Goldberg ("dark_sylinc")
//  Under MIT License
//  This file is part of Yangen.
//----------------------------------------------------------------------

#include "Core/VisualizationPanelImpl.h"

#include "Core/YangenWindowImpl.h"

VisualizationPanelImpl::VisualizationPanelImpl( YangenWindowImpl *yangenWindowImpl ) :
	VisualizationPanel( yangenWindowImpl ),
	m_yangenWindowImpl( yangenWindowImpl )
{
}
//-----------------------------------------------------------------------------
void VisualizationPanelImpl::OnChoice( wxCommandEvent &event )
{
	wxChoice *choice = static_cast<wxChoice *>( event.GetEventObject() );

	if( choice == m_visualizationChoice )
	{
		int selection = m_visualizationChoice->GetSelection();
		m_yangenWindowImpl->setVisualizationMode( static_cast<uint8_t>( selection ) );
	}
}
