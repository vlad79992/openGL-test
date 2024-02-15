#include "Test.h"

#include "../imgui/imgui.h"

test::TestMenu::TestMenu(Test*& curTestPtr)
	: m_CurrentTest(curTestPtr)
{
	
}

void test::TestMenu::OnUpdate(float deltaTime)
{
}

void test::TestMenu::OnRender()
{
}

void test::TestMenu::OnImGuiRender()
{
	for (auto& test : m_Tests)
	{
		if (ImGui::Button(test.first.c_str()))
		{
			m_CurrentTest = test.second();
		}
	}
}
