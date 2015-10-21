return
{
	{
		builder = "MeshBuilder.exe",
		assets = 
		{
			{source = "rectangle.msh", target = "rectangle.msh"},
			{source = "triangle.msh", target = "triangle.msh"},
		}
	},
	{
		builder = "GenericBuilder.exe",
		assets = 
		{
			{source = "vertex.shader", target = "vertex.shader"},
			{source = "fragment.shader", target = "fragment.shader"},
		}
	},
}