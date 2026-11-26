package app.organicmaps.bookmarks;

import android.content.Context;
import android.content.SharedPreferences;
import androidx.annotation.NonNull;
import app.organicmaps.MwmApplication;
import app.organicmaps.sdk.bookmarks.data.BookmarkCategory;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;

/**
 * Order of the bookmark lists: the sort mode and the pinned lists. Kept per device in preferences,
 * so pinning a list does not change its file.
 */
final class BookmarkCategoryOrder
{
  enum SortMode
  {
    NAME,
    IMPORT_DATE
  }

  private static final String KEY_SORT_MODE = "BookmarkCategoriesSortMode";
  private static final String KEY_PINNED = "BookmarkCategoriesPinned";

  private final SharedPreferences mPrefs;

  BookmarkCategoryOrder(@NonNull Context context)
  {
    mPrefs = MwmApplication.prefs(context);
  }

  @NonNull
  SortMode getSortMode()
  {
    final String value = mPrefs.getString(KEY_SORT_MODE, SortMode.NAME.name());
    try
    {
      return SortMode.valueOf(value);
    }
    catch (IllegalArgumentException e)
    {
      return SortMode.NAME;
    }
  }

  void setSortMode(@NonNull SortMode mode)
  {
    mPrefs.edit().putString(KEY_SORT_MODE, mode.name()).apply();
  }

  boolean isPinned(long categoryId)
  {
    return getPinnedIds().contains(String.valueOf(categoryId));
  }

  void togglePinned(long categoryId)
  {
    // The set returned by the preferences must not be modified, so work on a copy.
    final Set<String> pinned = new HashSet<>(getPinnedIds());
    final String key = String.valueOf(categoryId);
    if (!pinned.remove(key))
      pinned.add(key);
    mPrefs.edit().putStringSet(KEY_PINNED, pinned).apply();
  }

  /** Pinned lists first, then the rest, each group in the current sort mode. */
  @NonNull
  List<BookmarkCategory> sorted(@NonNull List<BookmarkCategory> categories)
  {
    final Comparator<BookmarkCategory> pinnedFirst = Comparator.comparing(c -> !isPinned(c.getId()));
    final Comparator<BookmarkCategory> byMode;
    if (getSortMode() == SortMode.NAME)
      byMode = Comparator.comparing(c -> c.getName().toLowerCase(Locale.ROOT));
    else
      byMode = Comparator.comparingLong(BookmarkCategory::getLastModifiedMs).reversed();

    final List<BookmarkCategory> result = new ArrayList<>(categories);
    result.sort(pinnedFirst.thenComparing(byMode));
    return result;
  }

  @NonNull
  private Set<String> getPinnedIds()
  {
    return mPrefs.getStringSet(KEY_PINNED, Collections.emptySet());
  }
}
