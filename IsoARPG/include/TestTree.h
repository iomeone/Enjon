#ifndef TEST_TREE_H
#define TEST_TREE_H

#include "BehaviorTree/BT.h"
#include "ECS/ComponentSystems.h"
#include "ECS/Transform3DSystem.h"
#include "ECS/AttributeSystem.h"
#include "ECS/EntityFactory.h"
#include "ECS/Renderer2DSystem.h"
#include "Math/Random.h"
#include <Enjon.h>

i32 RepeaterFunc(BT::BehaviorTree* T)
{
	auto D = static_cast<BlackBoardComponent<u32>*>(T->GetBlackBoard()->GetComponent("MovementLoop"));
	auto C = D->GetData();
	D->SetData(0);
	return C;
}

inline BT::BehaviorTree* TestTree()
{
	BT::BehaviorTree* BT = new BT::BehaviorTree();

	BT::Repeater* REP = new BT::Repeater(BT);
	BT::Sequence* SEQ = new BT::Sequence(BT);

	BT::SimpleTask* ST1 = new BT::SimpleTask(BT, [](BT::BehaviorTree* BT)
										   {
										   		auto D = static_cast<BlackBoardComponent<ECS::Systems::EntityManager*>*>(BT->GetBlackBoard()->GetComponent("EntityManager"));
										   		auto P = static_cast<BlackBoardComponent<EM::Vec3>*>(BT->GetBlackBoard()->GetComponent("TargetPosition"));
										   		auto Manager = D->GetData();

										   		auto W = Manager->Lvl->GetWidth();
										   		auto H = Manager->Lvl->GetHeight();

										   		// std::cout << W << ", " << H << "." << std::endl;

										   		auto X = ER::Roll(0, 800);
										   		auto Y = ER::Roll(-2000, -200);

										   		P->SetData(EM::Vec3(X, Y, 0.0f));

										   		// std::cout << "Setting new direction..." << std::endl;

										   		// Just set its velocity goal for now...
										   });

	BT::SimpleTask* ST2 = new BT::SimpleTask(BT, [](BT::BehaviorTree* BT)
										 {
									   		auto D = static_cast<BlackBoardComponent<ECS::Systems::EntityManager*>*>(BT->GetBlackBoard()->GetComponent("EntityManager"));
									   		auto ID = static_cast<BlackBoardComponent<ECS::eid32>*>(BT->GetBlackBoard()->GetComponent("EID"));
									   		auto P = static_cast<BlackBoardComponent<EM::Vec3>*>(BT->GetBlackBoard()->GetComponent("TargetPosition"));
									   		auto Manager = D->GetData();
									   		auto Target = P->GetData();
									   		auto ai = ID->GetData();

									   		auto TargetCartesian = EM::IsoToCartesian(Target.XY());
											
											auto AI = &Manager->TransformSystem->Transforms[ai];
									   		auto H = Manager->AttributeSystem->HealthComponents[ai].Health; 

											// Find difference in positions	
											Enjon::Math::Vec3 Difference = EM::Vec3::Normalize(EM::Vec3(TargetCartesian, Target.z) - 
																								EM::Vec3(AI->CartesianPosition, AI->Position.z));
											float speed = 5000.0f / (H + 0.0001f);

											AI->Velocity = Difference * speed;
											// AI->VelocityGoal = Difference * speed;
										 });

	BT::RepeaterWithBBRead* RWBBR = new BT::RepeaterWithBBRead(BT, 	&RepeaterFunc);														 

	BT::BBWrite* BBW = new BT::BBWrite(BT, [](BT::BehaviorTree* BT)
							{
						   		auto D = static_cast<BlackBoardComponent<u32>*>(BT->GetBlackBoard()->GetComponent("MovementLoop"));
						   		D->SetData(1);
							});

	BT::Inverter* INV = new BT::Inverter(BT);
	BT::ConditionalTask* CT = new BT::ConditionalTask(BT, [](BT::BehaviorTree* BT)
												   {
												   		auto D = static_cast<BlackBoardComponent<ECS::Systems::EntityManager*>*>(BT->GetBlackBoard()->GetComponent("EntityManager"));
												   		auto ID = static_cast<BlackBoardComponent<ECS::eid32>*>(BT->GetBlackBoard()->GetComponent("EID"));
												   		auto P = static_cast<BlackBoardComponent<EM::Vec3>*>(BT->GetBlackBoard()->GetComponent("TargetPosition"));
												   		auto Manager = D->GetData();
												   		auto Target = P->GetData();
												   		auto ai = ID->GetData();

												   		auto AI = &Manager->TransformSystem->Transforms[ai].Position.XY();
												   		auto V = &Manager->TransformSystem->Transforms[ai].Velocity.XY();
												   		auto H = Manager->AttributeSystem->HealthComponents[ai].Health; 

												   		auto Distance = AI->DistanceTo(Target.XY());


												   		// std::cout << Distance << std::endl;

												   		if (Distance <= 100.0f) 
												   		{
															auto AI2 = &Manager->TransformSystem->Transforms[ai];
															AI2->VelocityGoal = EM::Vec3(0.0f, 0.0f, 0.0f);
												   			return true;
												   		}
												   		
												   		else
												   		{
													   		auto TargetCartesian = EM::IsoToCartesian(Target.XY());
															
															auto AI2 = &Manager->TransformSystem->Transforms[ai];

															// Find difference in positions	
															Enjon::Math::Vec3 Difference = EM::Vec3::Normalize(EM::Vec3(TargetCartesian, Target.z) - 
																												EM::Vec3(AI2->CartesianPosition, AI2->Position.z));
															float speed = 5000.0f / (H + 0.0001f);

															AI2->Velocity = Difference * speed;
															// AI2->VelocityGoal = Difference * speed;

													   		return false;
												   		}	
												   });

	BT::WaitWBBRead* W = new BT::WaitWBBRead(BT);

	BT::SimpleTask* Shoot = new BT::SimpleTask(BT, [](BT::BehaviorTree* BT) 
													{
												   		auto Manager = static_cast<BlackBoardComponent<ECS::Systems::EntityManager*>*>(BT->GetBlackBoard()->GetComponent("EntityManager"))->GetData();
												   		auto ID = static_cast<BlackBoardComponent<ECS::eid32>*>(BT->GetBlackBoard()->GetComponent("EID"))->GetData();

												   		auto Position = Manager->TransformSystem->Transforms[ID].Position.XY();
														
														// Create an arrow projectile entity for now...
														static Enjon::Graphics::SpriteSheet ItemSheet;
														static float t = 0.0f;
														t += 0.1f;

														auto C = Enjon::Graphics::RGBA16(0.0f, 100.0f, 0.0f, 100.0f);
														C.r += 2.0f;
														if (!ItemSheet.IsInit()) ItemSheet.Init(Enjon::Input::ResourceManager::GetTexture("../IsoARPG/Assets/Textures/bluebutton.png"), Enjon::Math::iVec2(1, 1));
														ECS::eid32 id = ECS::Factory::CreateWeapon(Manager, Enjon::Math::Vec3(Position.x + 60.0f, Position.y + 40.0f, 50.0f),
																				  Enjon::Math::Vec2(10.0f + ER::Roll(0, 60), 5.0f), &ItemSheet, (ECS::Masks::Type::WEAPON | 
																				  												ECS::Masks::WeaponOptions::PROJECTILE | 
																				  												ECS::Masks::GeneralOptions::PICKED_UP),
																				  												ECS::Component::EntityType::PROJECTILE, "arrow", 
																																C);
														Manager->Masks[id] |= ECS::ComponentMasks::COMPONENT_TRANSFORM3D;

														Manager->TransformSystem->Transforms[id].AABBPadding = EM::Vec2(15);


														// Set arrow velocity to normalize: mousepos - arrowpos
														auto PlayerPosition = Manager->TransformSystem->Transforms[Manager->Player].Position.XY();

														// // Find vector between the two and normalize
														Enjon::Math::Vec2 ArrowVelocity = Enjon::Math::Vec2::Normalize(Enjon::Math::IsoToCartesian(PlayerPosition) - Enjon::Math::IsoToCartesian(Position));

														EM::Vec2 R(1,0);
														float a = acos(ArrowVelocity.DotProduct(R)) * 180.0f / M_PI;
														if (ArrowVelocity.y < 0) a *= -1;

														Manager->TransformSystem->Transforms[id].Angle = EM::ToRadians(a);

														auto ArrowX = Position.x;
														auto ArrowY = Position.y;
														auto Rad = 5.0f;
														EM::Vec3* ArrowPos = &Manager->TransformSystem->Transforms[id].Position;
														*ArrowPos = EM::Vec3(ArrowPos->XY() + Rad * EM::CartesianToIso(EM::Vec2(cos(EM::ToRadians(a)), sin(EM::ToRadians(a)))), 40.0f);
														
														auto RX = sin(t) * Enjon::Random::Roll(-10, 2) / 100.0f;
														auto RY = sin(t) * Enjon::Random::Roll(-10, 2) / 100.0f;
														ArrowVelocity = Enjon::Math::CartesianToIso(ArrowVelocity);

														float speed = 50.0f;

														// // Fire in direction of mouse
														Manager->TransformSystem->Transforms[id].VelocityGoal = speed * Enjon::Math::Vec3(ArrowVelocity.x + RX, ArrowVelocity.y + RY, 0.0f);
														Manager->TransformSystem->Transforms[id].Velocity = speed * Enjon::Math::Vec3(ArrowVelocity.x + RX, ArrowVelocity.y + RY, 0.0f);
														Manager->TransformSystem->Transforms[id].Velocity.z = -5.0f;
														Manager->TransformSystem->Transforms[id].BaseHeight = 0.0f;

														// Set up coordinate format
														Manager->Renderer2DSystem->Renderers[id].Format = EG::CoordinateFormat::ISOMETRIC;	

													});

	// Build tree
	BT->SetRoot(REP);
		REP->AddChild(SEQ);
			SEQ->AddChild(ST1);
			SEQ->AddChild(ST2);
			SEQ->AddChild(RWBBR);
				RWBBR->AddChild(BBW);
					BBW->AddChild(INV);
						INV->AddChild(CT);
			SEQ->AddChild(W);
			SEQ->AddChild(Shoot);
	BT->End();

	return BT;
}


#endif










