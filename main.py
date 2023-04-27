import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import datetime as dt
from collections import defaultdict

def parse_input(filename):
    entries = []
    min_date = dt.datetime(1, 1, 1)
    max_date = dt.datetime(9999, 12, 31)
    with open(filename, "r") as file:
        n = int(file.readline())
        for line in file:
            start_date, end_date, transport = line.strip().split()
            start_date = dt.datetime.strptime(start_date, "%d.%m.%Y")
            end_date = dt.datetime.strptime(end_date, "%d.%m.%Y")

            # Filter out entries with dates outside the valid range for Matplotlib
            if start_date >= min_date and end_date <= max_date:
                entries.append((start_date, end_date, transport.replace('_', ' ')))

    return entries

def visualize_timeline(entries, best_day):
    fig, ax = plt.subplots(figsize=(10, 5))
    for entry in entries:
        plt.hlines(y=entry[2], xmin=entry[0], xmax=entry[1], colors='b', linewidth=3)
    plt.axvline(best_day, color='r', linestyle='--')  # Use the best_day_datetime variable instead of best_day

    plt.xlabel('Date')
    plt.ylabel('Transport')
    plt.title('Transport Timeline')

    ax.xaxis.set_major_locator(mdates.YearLocator())
    ax.xaxis.set_minor_locator(mdates.MonthLocator())
    ax.xaxis.set_major_formatter(mdates.DateFormatter('%Y'))
    ax.xaxis.set_minor_formatter(mdates.DateFormatter('%b'))

    plt.xticks(fontsize=10)
    plt.yticks(fontsize=10)

    plt.show()


def find_best_day(entries):
    date_counter = defaultdict(int)

    for entry in entries:
        start_date, end_date = entry[0], entry[1]

        # Increment the counter for the start date
        date_counter[start_date] += 1

        # Decrement the counter for the day after the end date
        day_after_end_date = end_date + dt.timedelta(days=1)
        date_counter[day_after_end_date] -= 1

    max_count = -1
    current_count = 0
    best_day = None

    # Sort the dates in the date_counter
    sorted_dates = sorted(date_counter)

    # Iterate through the sorted dates
    for date in sorted_dates:
        current_count += date_counter[date]
        if current_count > max_count:
            max_count = current_count
            best_day = date
    print(best_day)
    return best_day



if __name__ == "__main__":
    entries = parse_input("input.txt")
    best_day = find_best_day(entries)
    visualize_timeline(entries, best_day)