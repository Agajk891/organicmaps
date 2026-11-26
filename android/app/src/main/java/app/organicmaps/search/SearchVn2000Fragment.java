package app.organicmaps.search;

import android.os.Bundle;
import android.text.Editable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import app.organicmaps.R;
import app.organicmaps.sdk.util.StringUtils;
import java.util.Locale;

/**
 * VN2000 tab of the search sheet: a province or city and an X/Y pair in VN-2000 metres.
 * "Show on map" searches the query "VN2000 <province> <X> <Y>", which the core parses into a pin.
 */
public class SearchVn2000Fragment extends Fragment
{
  // VN-2000 coordinates are positive decimals.
  private static final String NUMBER_PATTERN = "\\d+(\\.\\d+)?";

  private Spinner mProvince;
  private EditText mX;
  private EditText mY;
  private Button mShow;

  @Nullable
  @Override
  public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container,
                           @Nullable Bundle savedInstanceState)
  {
    return inflater.inflate(R.layout.fragment_search_vn2000, container, false);
  }

  @Override
  public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState)
  {
    super.onViewCreated(view, savedInstanceState);
    mProvince = view.findViewById(R.id.vn2000_province);
    mX = view.findViewById(R.id.vn2000_x);
    mY = view.findViewById(R.id.vn2000_y);
    mShow = view.findViewById(R.id.vn2000_show);

    final String[] provinces = view.getResources().getStringArray(R.array.vn2000_provinces);
    mProvince.setAdapter(
        new ArrayAdapter<>(view.getContext(), android.R.layout.simple_spinner_dropdown_item, provinces));

    final StringUtils.SimpleTextWatcher watcher = new StringUtils.SimpleTextWatcher() {
      @Override
      public void afterTextChanged(Editable s)
      {
        updateShowButton();
      }
    };
    mX.addTextChangedListener(watcher);
    mY.addTextChangedListener(watcher);
    mShow.setOnClickListener(v -> showOnMap());
    updateShowButton();
  }

  private void updateShowButton()
  {
    mShow.setEnabled(readNumber(mX) != null && readNumber(mY) != null);
  }

  /** Returns the field as a plain decimal with a point, or null if it is not one. */
  @Nullable
  private static String readNumber(@NonNull EditText field)
  {
    // Vietnamese keyboards type a decimal comma; the core parser expects a point.
    final String text = field.getText().toString().trim().replace(',', '.');
    return text.matches(NUMBER_PATTERN) ? text : null;
  }

  private void showOnMap()
  {
    final String x = readNumber(mX);
    final String y = readNumber(mY);
    final Object province = mProvince.getSelectedItem();
    if (x == null || y == null || province == null)
      return;

    final String query = String.format(Locale.US, "VN2000 %s %s %s", province, x, y);
    final Fragment parent = getParentFragment();
    if (parent instanceof SearchFragment)
      ((SearchFragment) parent).setQuery(query, false);
  }
}
