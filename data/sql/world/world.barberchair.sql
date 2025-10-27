
-- 
-- world.gameobject
-- 

UPDATE `gameobject` 
JOIN `gameobject_template` ON `gameobject`.`id` = `gameobject_template`.`entry` 
SET `gameobject`.`ScriptName` = 'mod_barberchair' 
WHERE `gameobject_template`.`type` = 32; -- GAMEOBJECT_TYPE_BARBER_CHAIR
