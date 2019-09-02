#include "DefaultSwordSlashHitbox.h"

#include "LevelManager.h"
#include "BaseEnemy.h"
#include "HelperFunctions.h"

lbe::RegisterAsSceneObject<lbe::DefaultSwordSlashHitbox> lbe::DefaultSwordSlashHitbox::RegisterSceneObject("DefaultSwordSlashHitbox");

lbe::DefaultSwordSlashHitbox::DefaultSwordSlashHitbox(sf::Vector2f Pos) : SceneObject(Pos)
{
	Name = "DefaultSwordSlashHitbox";
}


lbe::DefaultSwordSlashHitbox::~DefaultSwordSlashHitbox()
{
}

bool lbe::DefaultSwordSlashHitbox::ApplyUpdatePacket(sf::Packet & packet)
{
	return packet >> Damage >> Rotation >> ArcSize;
}

bool lbe::DefaultSwordSlashHitbox::WriteUpdatePacket(sf::Packet & packet)
{
	return packet << Damage << Rotation << ArcSize;
}

void lbe::DefaultSwordSlashHitbox::Draw(sf::RenderWindow & window)
{
}

void lbe::DefaultSwordSlashHitbox::Update(sf::Time dt)
{
	for (auto &Object : LevelManager::GetSceneObjectsFromLevel(Position))
	{
		if (BaseEnemy* Enemy = dynamic_cast<BaseEnemy*>(Object))
		{
			//LBE_TRACE("Checking for hit with enemy!");
			double DistanceBetween = lbe::HelperFunctions::GetDistance(Position, Enemy->GetPos());
			//LBE_TRACE("DistanceBetween={0}", DistanceBetween);
			if (DistanceBetween > lbe::PLAYER_RADIUS && DistanceBetween < lbe::PLAYER_SLASH_REACH)
			{
				//LBE_TRACE("Enemy is close enough!");
				sf::Vector2f EnemyVector = (Enemy->GetPos() - Position) / (sqrt(pow(abs(Enemy->GetPos().x - Position.x), 2) + pow(abs(Enemy->GetPos().y - Position.y), 2)));
			
				double EnemyRotation = lbe::HelperFunctions::GetRotation(EnemyVector);
				double RotationDifference = Rotation - EnemyRotation;
				RotationDifference = abs(RotationDifference);
				//LBE_TRACE("RotationDifference={0}", RotationDifference);
				if (RotationDifference < ArcSize)
				{
					Enemy->ReceiveDamage(Damage);
					//LBE_TRACE("Enemy received damage!");
				}
			}
		}
	}

	MarkForDeletion();
	//LBE_TRACE("SwordSlashHitbox update finished!");
}

void lbe::DefaultSwordSlashHitbox::SetRotation(double NewRotation)
{
	Rotation = NewRotation;
}