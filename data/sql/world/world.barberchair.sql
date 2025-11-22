-- 
-- world.gameobject
-- 

UPDATE `gameobject` 
JOIN `gameobject_template` ON `gameobject`.`id` = `gameobject_template`.`entry` 
SET `gameobject`.`ScriptName` = 'mod_barberchair' 
WHERE `gameobject_template`.`type` = 32; -- type 32 = GAMEOBJECT_TYPE_BARBER_CHAIR

/* Commented line is needed if you want to revert the changes and make a clean delete of this module */
-- UPDATE `gameobject`SET `ScriptName` = '' WHERE `ScriptName` = 'mod_barberchair';
