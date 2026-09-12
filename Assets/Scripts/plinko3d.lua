-- 3D Plinko Game Script
-- Example Lua script for controlling 3D Plinko gameplay
-- Requires LuaScriptComponent to be re-enabled in RCEngine

-- Game state
local ballEntity = nil
local spawnTimer = 0.0
local spawnInterval = 2.0
local score = 0
local bucketScores = {10, 20, 50, 20, 10}

-- Ball spawn position
local spawnPos = {x = 0, y = 7, z = 0}
local spawnVariance = 2.0

-- Initialize function (called when script is loaded)
function OnCreate()
    print("3D Plinko initialized!")
    ballEntity = GetEntityByName("Ball")
    
    if ballEntity then
        print("Ball entity found")
    else
        print("Warning: Ball entity not found")
    end
end

-- Update function (called every frame)
function OnUpdate(deltaTime)
    spawnTimer = spawnTimer + deltaTime
    
    -- Spawn new ball every few seconds
    if spawnTimer >= spawnInterval then
        SpawnBall()
        spawnTimer = 0.0
    end
    
    -- Check for balls that fell below the floor
    CheckBallCollections()
end

-- Spawn a new ball at the top with random X offset
function SpawnBall()
    if not ballEntity then
        return
    end
    
    -- Random X position
    local randomX = (math.random() - 0.5) * spawnVariance
    
    -- Reset ball position
    SetEntityPosition(ballEntity, spawnPos.x + randomX, spawnPos.y, spawnPos.z)
    
    -- Give it an initial velocity (when physics is enabled)
    -- SetEntityVelocity(ballEntity, 0, -1, 0)
    
    print(string.format("Ball spawned at x=%.2f", spawnPos.x + randomX))
end

-- Check which bucket the ball fell into
function CheckBallCollections()
    if not ballEntity then
        return
    end
    
    local pos = GetEntityPosition(ballEntity)
    
    -- If ball is below bucket level
    if pos.y < -6.0 then
        local bucketIndex = DetermineBucket(pos.x)
        
        if bucketIndex > 0 and bucketIndex <= #bucketScores then
            local points = bucketScores[bucketIndex]
            score = score + points
            print(string.format("Ball landed in bucket %d! +%d points (Total: %d)", 
                bucketIndex, points, score))
        end
        
        -- Respawn the ball
        SpawnBall()
    end
end

-- Determine which bucket the ball is in based on X position
function DetermineBucket(x)
    -- Bucket positions: -6, -3, 0, 3, 6
    if x < -4.5 then
        return 1
    elseif x < -1.5 then
        return 2
    elseif x < 1.5 then
        return 3
    elseif x < 4.5 then
        return 4
    else
        return 5
    end
end

-- Handle input (called when key is pressed)
function OnKeyPressed(key)
    -- R key to reset
    if key == KEY_R then
        ResetGame()
    end
    
    -- Space to spawn ball manually
    if key == KEY_SPACE then
        SpawnBall()
    end
    
    -- Number keys to set spawn position
    if key == KEY_1 then
        spawnPos.x = -4
    elseif key == KEY_2 then
        spawnPos.x = -2
    elseif key == KEY_3 then
        spawnPos.x = 0
    elseif key == KEY_4 then
        spawnPos.x = 2
    elseif key == KEY_5 then
        spawnPos.x = 4
    end
end

-- Reset the game
function ResetGame()
    score = 0
    spawnTimer = 0.0
    print("Game reset! Score: 0")
    SpawnBall()
end

-- Cleanup function (called when script is destroyed)
function OnDestroy()
    print("3D Plinko script destroyed")
end

-- Helper functions (these would be provided by the engine)
-- Placeholder implementations for reference

function GetEntityByName(name)
    -- Engine function to find entity by tag
    return {name = name}
end

function SetEntityPosition(entity, x, y, z)
    -- Engine function to set entity position
    print(string.format("Set %s position to (%.2f, %.2f, %.2f)", entity.name, x, y, z))
end

function GetEntityPosition(entity)
    -- Engine function to get entity position
    return {x = 0, y = 7, z = 0}
end

function SetEntityVelocity(entity, vx, vy, vz)
    -- Engine function to set physics velocity
    print(string.format("Set %s velocity to (%.2f, %.2f, %.2f)", entity.name, vx, vy, vz))
end

-- Key codes (would be defined by engine)
KEY_R = 82
KEY_SPACE = 32
KEY_1 = 49
KEY_2 = 50
KEY_3 = 51
KEY_4 = 52
KEY_5 = 53
