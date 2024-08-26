import { VehicleData } from "./server";

/**
 * 
 * @param jsonData 
 * @param tempExceedRecords 
 * @returns true if received temperatures exceeds the [20,80] boundary more than 3 times within 5 seconds, false otherwise.
 */
export function tempExceed(jsonData: VehicleData, tempExceedRecords: number[]) {
	let temperature = jsonData.battery_temperature;

	if (temperature < 20 || temperature > 80) {
		let now = (Date.now());
		// Remove all records that is not within 5 seconds
		while (tempExceedRecords.length > 0 && now - tempExceedRecords.indexOf(0) > 5000) {
		tempExceedRecords.shift();
		}
		
		tempExceedRecords.push(now);
		if (tempExceedRecords.length > 3) {
				return true;
		}
	}
	return false;
}
