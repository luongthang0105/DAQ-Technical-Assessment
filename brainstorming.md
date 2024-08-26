# Brainstorming

This file is used to document your thoughts, approaches and research conducted across all tasks in the Technical Assessment.

## Firmware

## Telemetry

### Task 1

The server crashes occasionally because it fails to parse the data from the data-emulator.

The terminal also shows the string that causes the system to fail, and it seems to have an excessive '{' at the end. Hence, I try to investigate **battery_emulator.ts** to see where would that extra brace comes from.

In the **_generate_and_send_battery_data()_** function, there's a suspicious piece of code:

```typescript
if (error_flag === 3) {
  json_string += "}"
}
```

Removing this would prevent the server from crashing.

I went to another problem is that the **_docker compose up_** command does not load changed files. Doing some research I know that I should have ran **_docker compose build_** to build the images first, then **_docker compose up_** to build upon the new images.

### Task 2

I made another file called **exceptions.ts** to handle this feature. In particular, I created an array to store the timestamps where an out-of-bound temperature is received.

Then if the received temperatures in this array is outdated (ie. 5 seconds later than the most recent one) then I would remove them from the array. If the result array (including the newly received temperature) length exceeds 3 then I will print an error message to console.

To test this out, I reduce the **ERROR_CHANCE** constant in **battery_emulator.ts** to make sure it randomizes to the out-of-range option more.

### Task 3

I added a function in **live_value.tsx** to determine the color of the battery value, based on the input temperature.

I tried to change the frontend a bit by first replacing the Redback logo with the new one so now it looks kinda cool :P I changed the "Live Battery Temperature" font so it looks a bit more aesthetic. Finally I decorated the value box so it catches attention more, and it looks more formal as well i think.

## Cloud
