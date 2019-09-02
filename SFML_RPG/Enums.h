#pragma once

namespace lbe
{
	enum Orientation
	{
		Horizontal,
		Vertical
	};

	enum ButtonState
	{
		Deselected,
		Selected,
		Clicked
	};

	enum Layer
	{
		Background,
		Collision,
		Object,
		Meta
	};

	enum NetworkState
	{
		Host,
		Client
	};
}