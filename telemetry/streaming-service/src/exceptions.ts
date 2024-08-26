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
		let now = jsonData.timestamp;
		// Remove all records that is not within 5 seconds
		while (true) {
			let earliestRecord = tempExceedRecords.at(0);
			if (earliestRecord != undefined && now - earliestRecord > 5000) {
				tempExceedRecords.shift();
			} else {
				break;
			}
		}
		tempExceedRecords.push(now);
		if (tempExceedRecords.length > 3) {
				return {
					records: tempExceedRecords,
					isError: true
				}
		}
	}
	return {
		records: tempExceedRecords,
		isError: false
	}
}
