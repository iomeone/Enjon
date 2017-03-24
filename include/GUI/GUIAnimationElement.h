#ifndef ENJON_GUI_ANIMATION_ELEMENT_H
#define ENJON_GUI_ANIMATION_ELEMENT_H

#include "GUI/GUIElement.h"
#include "Graphics/Animations.h"
#include "Math/Vec2.h"
#include "Defines.h" 

namespace Enjon { namespace GUI {

	// AnimationFrameElement
	struct GUIAnimationElement : GUIElement<GUIAnimationElement>
	{
		GUIAnimationElement()
		{
			// Set up type
			this->Type = GUIType::SCENE_ANIMATION;

			// Set up states
			this->State = ButtonState::INACTIVE;
			this->HoverState = HoveredState::OFF_HOVER;

			// Set up member variables
			this->CurrentIndex = 0;
			this->CurrentAnimation = nullptr;

			// Set up SceneAnimation's on_hover signal
			this->on_hover.connect([&]()
			{
				this->HoverState = HoveredState::ON_HOVER;
			});

			// Set up SceneAnimation's off_hover signal
			this->off_hover.connect([&]()
			{
				this->HoverState = HoveredState::OFF_HOVER;
			});

		}

		void Init() {}

		void Update()
		{
			
		}

		bool ProcessInput(Input* Input, Camera2D* Camera)
		{
			static Enjon::Vec2 MouseFrameOffset(0.0f);
			static bool JustFocused = true;

		    SDL_Event event;
		    while (SDL_PollEvent(&event)) 
		    {
		        switch (event.type) 
		        {
					case SDL_KEYUP:
						Input->ReleaseKey(event.key.keysym.sym); 
						break;
					case SDL_KEYDOWN:
						Input->PressKey(event.key.keysym.sym);
						break;
					case SDL_MOUSEBUTTONDOWN:
						Input->PressKey(event.button.button);
						break;
					case SDL_MOUSEBUTTONUP:
						Input->ReleaseKey(event.button.button);
						break;
					case SDL_MOUSEMOTION:
						Input->SetMouseCoords((float)event.motion.x, (float)event.motion.y);
						break;
					default:
						break;
				}
		    }

		    auto MousePos = Input->GetMouseCoords();
		    Camera->ConvertScreenToWorld(MousePos);

		    if (Input->IsKeyDown(SDL_BUTTON_LEFT))
		    {
				auto X = MousePos.x;
				auto Y = MousePos.y;

	    		if (JustFocused) 
	    		{
	    			MouseFrameOffset = Enjon::Vec2(MousePos.x - this->AABB.Min.x, MousePos.y - this->AABB.Min.y);
	    			JustFocused = false;

	    			// Just clicked
	    			this->on_click.emit();
	    		}

				// Update offsets
				this->CurrentAnimation->Frames.at(this->CurrentIndex).Offsets = Enjon::Vec2(X - MouseFrameOffset.x, Y - MouseFrameOffset.y);

				// Emit that value has changed
				this->on_value_change.emit();
		    }

	    	else 
	    	{
	    		this->lose_focus.emit();
	    		JustFocused = true;
	    		return true;
	    	}

			return false;
		}

		void Draw(SpriteBatch* TB)
		{

		}

		EA::Anim* CurrentAnimation;
		uint32_t CurrentIndex;
		EGUI::Signal<> on_value_change;
	};

	namespace AnimationElement
	{
		void AABBSetup(GUIAnimationElement* SceneAnimation);
	}
}}


#endif